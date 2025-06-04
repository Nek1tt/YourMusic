#include "home-handler.h"
#include <iostream>
#include <unordered_set>
#include <algorithm> // для std::sort

// Логируем только ошибки MySQL (если понадобятся)
static void log_mysql_error(const boost::mysql::error_code& ec,
                            const boost::mysql::diagnostics& diag,
                            const std::string& where)
{
    std::cerr << "[HomeHandler] " << where << " error: " << ec.message() << "\n";
}

http::response<http::string_body> HomeHandler::operator()(int http_version) const {
    // Подготовим базовый успешный ответ
    http::response<http::string_body> res{ http::status::ok, http_version };
    res.set(http::field::content_type, "application/json");

    json resp_body;
    resp_body["status"] = "ok";

    // 1) Получаем список всех прямых подписок (level 1)
    std::vector<std::string> target_tags = db_.get_target_usertags_by_usertag(usertag_);
    if (target_tags.empty()) {
        // Если подписок нет — просто возвращаем пустые массивы
        resp_body["new_albums"] = json::array();
        resp_body["new_tracks"] = json::array();
        resp_body["recomendations_albums"] = json::array();
        resp_body["recomendations_tracks"] = json::array();
        res.body() = resp_body.dump();
        res.prepare_payload();
        return res;
    }

    // =========================
    // 2) Собираем «новые» альбомы/треки для level 1
    // =========================
    json new_albums = json::array();
    json new_tracks = json::array();

    // для того, чтобы не дублировать треки, уже включённые в альбомы
    std::unordered_set<int> tracks_in_albums;

    for (auto const& author_tag : target_tags) {
        // --- 2.1: альбомы этого автора ---
        std::vector<int> album_ids = db_.get_album_ids_by_usertag(author_tag);
        for (int album_id : album_ids) {
            auto album_opt = db_.get_album_info(album_id);
            if (!album_opt.has_value()) {
                continue;
            }
            json album = album_opt.value();

            // получаем треки этого альбома
            json tracks = db_.get_tracks_for_album(album_id);

            // пометим каждый трек из альбома
            for (auto const& t : tracks) {
                if (t.contains("id") && t["id"].is_number_integer()) {
                    tracks_in_albums.insert(t["id"].get<int>());
                }
            }

            // добавляем список треков к объекту альбома
            album["tracks"] = std::move(tracks);
            album["author_usertag"] = author_tag; // указываем автора
            new_albums.push_back(std::move(album));
        }

        // --- 2.2: отдельные треки этого автора, не вошедшие в альбомы ---
        std::vector<int> track_ids = db_.get_track_ids_by_usertag(author_tag);
        for (int track_id : track_ids) {
            if (tracks_in_albums.count(track_id)) {
                continue;
            }
            auto track_opt = db_.get_track_info(track_id);
            if (!track_opt.has_value()) {
                continue;
            }
            json track = track_opt.value();
            track["author_usertag"] = author_tag;
            new_tracks.push_back(std::move(track));
        }
    }

    // ===== Сортировка new_albums по полю "created_date" (ISO YYYY-MM-DD) =====
    std::sort(new_albums.begin(), new_albums.end(),
        [](const json& a, const json& b) {
            // предполагаем, что оба поля существуют и имеют строковый формат "YYYY-MM-DD"
            return a["created_date"].get<std::string>() >
                   b["created_date"].get<std::string>();
        });

    // ===== Сортировка new_tracks по полю "upload_date" (ISO YYYY-MM-DD) =====
    std::sort(new_tracks.begin(), new_tracks.end(),
        [](const json& a, const json& b) {
            return a["upload_date"].get<std::string>() >
                   b["upload_date"].get<std::string>();
        });

    resp_body["new_albums"] = std::move(new_albums);
    resp_body["new_tracks"] = std::move(new_tracks);


    // =========================
    // 3) Строим «рекомендации» (level 2)
    // =========================

    // 3.1) Сначала получаем для каждого автора из level 1 того, на кого они подписаны
    std::unordered_set<std::string> level2_set;
    for (auto const& first_level_tag : target_tags) {
        // получаем список подписок автора автор
        std::vector<std::string> second_targets = db_.get_target_usertags_by_usertag(first_level_tag);
        for (auto const& second_tag : second_targets) {
            level2_set.insert(second_tag);
        }
    }

    // 3.2) Исключаем тех, кто уже в level 1, а также самого себя
    std::unordered_set<std::string> level1_set(target_tags.begin(), target_tags.end());
    level2_set.erase(usertag_);
    for (auto const& t1 : level1_set) {
        level2_set.erase(t1);
    }

    // 3.3) Переводим level2_set обратно в вектор для итерирования
    std::vector<std::string> recommendation_tags;
    recommendation_tags.reserve(level2_set.size());
    for (auto const& tag : level2_set) {
        recommendation_tags.push_back(tag);
    }

    // 3.4) Если рекомендаций нет, сразу кладём пустые массивы
    if (recommendation_tags.empty()) {
        resp_body["recomendations_albums"] = json::array();
        resp_body["recomendations_tracks"] = json::array();
        res.body() = resp_body.dump();
        res.prepare_payload();
        return res;
    }

    // =========================
    // 4) Собираем «новые» альбомы/треки для рекомендаций
    // =========================
    json rec_albums = json::array();
    json rec_tracks = json::array();
    std::unordered_set<int> rec_tracks_in_albums;

    for (auto const& author_tag : recommendation_tags) {
        // --- 4.1: альбомы рекомендованного автора ---
        std::vector<int> album_ids = db_.get_album_ids_by_usertag(author_tag);
        for (int album_id : album_ids) {
            auto album_opt = db_.get_album_info(album_id);
            if (!album_opt.has_value()) {
                continue;
            }
            json album = album_opt.value();

            // получаем треки альбома
            json tracks = db_.get_tracks_for_album(album_id);

            // помечаем треки
            for (auto const& t : tracks) {
                if (t.contains("id") && t["id"].is_number_integer()) {
                    rec_tracks_in_albums.insert(t["id"].get<int>());
                }
            }

            album["tracks"] = std::move(tracks);
            album["author_usertag"] = author_tag;
            rec_albums.push_back(std::move(album));
        }

        // --- 4.2: отдельные треки автора (не вошедшие в альбомы) ---
        std::vector<int> track_ids = db_.get_track_ids_by_usertag(author_tag);
        for (int track_id : track_ids) {
            if (rec_tracks_in_albums.count(track_id)) {
                continue;
            }
            auto track_opt = db_.get_track_info(track_id);
            if (!track_opt.has_value()) {
                continue;
            }
            json track = track_opt.value();
            track["author_usertag"] = author_tag;
            rec_tracks.push_back(std::move(track));
        }
    }

    resp_body["recomendations_albums"] = std::move(rec_albums);
    resp_body["recomendations_tracks"] = std::move(rec_tracks);

    // =========================
    // 5) Собираем и отдаем финальный JSON
    // =========================
    res.body() = resp_body.dump();
    res.prepare_payload();
    return res;
}
