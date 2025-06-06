#include "home-handler.h"
#include <iostream>
#include <unordered_set>
#include <algorithm>

static void log_mysql_error(const boost::mysql::error_code& ec,
                            const boost::mysql::diagnostics& diag,
                            const std::string& where)
{
    std::cerr << "[HomeHandler] " << where << " error: " << ec.message() << "\n";
}

http::response<http::string_body> HomeHandler::operator()(int http_version) const {
    http::response<http::string_body> res{ http::status::ok, http_version };
    res.set(http::field::content_type, "application/json");

    json resp_body;
    resp_body["status"] = "ok";

    // 1) Получаем список всех прямых подписок (level 1)
    std::vector<std::string> target_tags = db_.get_target_usertags_by_usertag(usertag_);

    json new_albums = json::array();
    json new_tracks = json::array();
    json rec_albums = json::array();
    json rec_tracks = json::array();
    json random_tracks = json::array();

    // =========================
    // 2) Если есть подписки, собираем «новые» альбомы/треки (level 1)
    // =========================
    std::unordered_set<int> tracks_in_albums;
    if (!target_tags.empty()) {
        for (auto const& author_tag : target_tags) {
            // 2.1) Альбомы этого автора
            std::vector<int> album_ids = db_.get_album_ids_by_usertag(author_tag);
            for (int album_id : album_ids) {
                auto album_opt = db_.get_album_info(album_id);
                if (!album_opt.has_value()) continue;
                json album = album_opt.value();

                // Получаем треки альбома
                json tracks = db_.get_tracks_for_album(album_id);
                for (auto const& t : tracks) {
                    if (t.contains("id") && t["id"].is_number_integer()) {
                        tracks_in_albums.insert(t["id"].get<int>());
                    }
                }

                album["tracks"] = std::move(tracks);
                album["author_usertag"] = author_tag;
                new_albums.push_back(std::move(album));
            }

            // 2.2) Отдельные треки этого автора, не вошедшие в альбомы
            std::vector<int> track_ids = db_.get_track_ids_by_usertag(author_tag);
            for (int track_id : track_ids) {
                if (tracks_in_albums.count(track_id)) continue;
                auto track_opt = db_.get_track_info(track_id);
                if (!track_opt.has_value()) continue;
                json track = track_opt.value();
                track["author_usertag"] = author_tag;
                new_tracks.push_back(std::move(track));
            }
        }

        std::sort(new_albums.begin(), new_albums.end(),
            [](const json& a, const json& b) {
                return a["created_date"].get<std::string>() >
                       b["created_date"].get<std::string>();
            });
    
        std::sort(new_tracks.begin(), new_tracks.end(),
            [](const json& a, const json& b) {
                return a["upload_date"].get<std::string>() >
                       b["upload_date"].get<std::string>();
            });
    }

    resp_body["new_albums"] = std::move(new_albums);
    resp_body["new_tracks"] = std::move(new_tracks);

    // =========================
    // 3) Строим «рекомендации» (level 2)
    // =========================
    std::unordered_set<std::string> level2_set;
    if (!target_tags.empty()) {
        for (auto const& first_tag : target_tags) {
            std::vector<std::string> second_targets = db_.get_target_usertags_by_usertag(first_tag);
            for (auto const& st : second_targets) {
                level2_set.insert(st);
            }
        }
        // Исключаем level 1 и самого себя
        std::unordered_set<std::string> level1_set(target_tags.begin(), target_tags.end());
        level2_set.erase(usertag_);
        for (auto const& t1 : level1_set) {
            level2_set.erase(t1);
        }
    }

    std::vector<std::string> recommendation_tags;
    recommendation_tags.reserve(level2_set.size());
    for (auto const& tag : level2_set) {
        recommendation_tags.push_back(tag);
    }

    // Если recommendation_tags пуст, rec_albums и rec_tracks останутся пустыми
    if (!recommendation_tags.empty()) {
        std::unordered_set<int> rec_tracks_in_albums;
        for (auto const& author_tag : recommendation_tags) {
            // 4.1) Альбомы рекомендованного автора
            std::vector<int> album_ids = db_.get_album_ids_by_usertag(author_tag);
            for (int album_id : album_ids) {
                auto album_opt = db_.get_album_info(album_id);
                if (!album_opt.has_value()) continue;
                json album = album_opt.value();

                // Получаем треки альбома
                json tracks = db_.get_tracks_for_album(album_id);
                for (auto const& t : tracks) {
                    if (t.contains("id") && t["id"].is_number_integer()) {
                        rec_tracks_in_albums.insert(t["id"].get<int>());
                    }
                }

                album["tracks"]         = std::move(tracks);
                album["author_usertag"] = author_tag;
                rec_albums.push_back(std::move(album));
            }

            // 4.2) Отдельные треки рекомендованного автора
            std::vector<int> track_ids = db_.get_track_ids_by_usertag(author_tag);
            for (int track_id : track_ids) {
                if (rec_tracks_in_albums.count(track_id)) continue;
                auto track_opt = db_.get_track_info(track_id);
                if (!track_opt.has_value()) continue;
                json track = track_opt.value();
                track["author_usertag"] = author_tag;
                rec_tracks.push_back(std::move(track));
            }
        }
    }

    resp_body["recomendations_albums"] = std::move(rec_albums);
    resp_body["recomendations_tracks"] = std::move(rec_tracks);

    // 5.1) Собираем set<int> exclude_ids из трех источников:
    //      • new_tracks
    //      • recomendations_tracks
    //      • лайки пользователя
    std::unordered_set<int> exclude_ids;
    for (auto const& t : resp_body["new_tracks"]) {
        if (t.contains("id") && t["id"].is_number_integer()) {
            exclude_ids.insert(t["id"].get<int>());
        }
    }
    for (auto const& t : resp_body["recomendations_tracks"]) {
        if (t.contains("id") && t["id"].is_number_integer()) {
            exclude_ids.insert(t["id"].get<int>());
        }
    }
    std::vector<int> liked_ids = db_.get_liked_track_ids(usertag_);
    for (int id : liked_ids) {
        exclude_ids.insert(id);
    }

    std::vector<int> exclude_vec;
    exclude_vec.reserve(exclude_ids.size());
    for (int ex : exclude_ids) {
        exclude_vec.push_back(ex);
    }

    // 5.2) Запрашиваем до 5 случайных треков, удовлетворяющих всем условиям
    std::vector<json> rand_list = db_.get_random_tracks(exclude_vec, 5);

    // 5.3) Кладём их в resp_body["random_tracks"]
    for (auto& obj : rand_list) {
        random_tracks.push_back(std::move(obj));
    }
    resp_body["random_tracks"] = std::move(random_tracks);

    res.body() = resp_body.dump();
    res.prepare_payload();
    return res;
}
