#include "profile-handler.h"

// Логируем только ошибки MySQL (если понадобятся)
static void log_mysql_error(const boost::mysql::error_code& ec,
                            const boost::mysql::diagnostics& diag,
                            const std::string& where)
{
    std::cerr << "[ProfileHandler] " << where << " error: " << ec.message() << "\n";
}

http::response<http::string_body> ProfileHandler::operator()(int http_version) const {
    // Подготовим ответ
    http::response<http::string_body> res{ http::status::ok, http_version };
    res.set(http::field::content_type, "application/json");

    json resp_body;
    resp_body["status"] = "ok";

    // 1) Получаем базовую инфу о пользователе (username + флаги приватности)
    auto user_info_opt = db_.get_user_profile_info(usertag_);
    if (!user_info_opt.has_value()) {
        resp_body = json{
            {"status", "error"},
            {"message", "User not found: " + usertag_}
        };
        res.body() = resp_body.dump();
        res.prepare_payload();
        return res;
    }
    UserProfileInfo ui = user_info_opt.value();

    // 2) Считаем подписки/подписчиков и треки
    int followersNum     = db_.count_followers(usertag_);
    int followingNum     = db_.count_following(usertag_);
    int tracksLoadedNum  = db_.count_tracks_loaded(usertag_);
    int tracksAddedNum   = db_.count_tracks_liked(usertag_);

    // Формируем блок user_info в ответе
    json user_info = {
        {"username",       ui.username},
        {"usertag",        usertag_},
        {"followersnum",   followersNum},
        {"followingnum",   followingNum},
        {"tracksLoadedNum", tracksLoadedNum},
        {"tracksAddedNum",  tracksAddedNum}
    };
    resp_body["user_info"] = std::move(user_info);

    // =========================
    // 3) Собираем список альбомов
    // =========================
    json albums_arr = json::array();
    // Если чужой профиль и приватность myAlbums == false, оставляем пустой массив
    if (!(flag_ == 1 && ui.myAlbums == false)) {
        // Иначе получаем все album_id, где usertag_ является автором
        std::vector<int> album_ids = db_.get_album_ids_by_usertag(usertag_);
        for (int album_id : album_ids) {
            auto album_opt = db_.get_album_info(album_id);
            if (!album_opt.has_value()) {
                continue;
            }
            json album = album_opt.value();
            // Перекладываем поля в нужный формат:
            json album_obj;
            album_obj["album_id"]        = album["id"];
            album_obj["title"]           = album["title"];
            album_obj["author_username"] = ui.username;
            album_obj["author_usertag"]  = usertag_;
            album_obj["track_count"]     = album["track_count"];
            // Вложенный массив треков:
            json tracks = db_.get_tracks_for_album(album_id);
            album_obj["tracks"] = std::move(tracks);
            albums_arr.push_back(std::move(album_obj));
        }
    }
    resp_body["albums"] = std::move(albums_arr);

    // =========================
    // 4) Собираем список лайкнутых треков
    // =========================
    json liked_tracks_arr = json::array();
    if (!(flag_ == 1 && ui.myTracks == false)) {
        std::vector<int> liked_ids = db_.get_liked_track_ids(usertag_);
        for (int tid : liked_ids) {
            auto track_opt = db_.get_track_info(tid);
            if (!track_opt.has_value()) {
                continue;
            }
            liked_tracks_arr.push_back(track_opt.value());
        }
    }
    resp_body["liked_tracks"] = std::move(liked_tracks_arr);

    // =========================
    // 5) Собираем список загруженных треков
    // =========================
    json loaded_tracks_arr = json::array();
    if (!(flag_ == 1 && ui.myLoadedTracks == false)) {
        std::vector<int> loaded_ids = db_.get_loaded_track_ids(usertag_);
        for (int tid : loaded_ids) {
            auto track_opt = db_.get_track_info(tid);
            if (!track_opt.has_value()) {
                continue;
            }
            loaded_tracks_arr.push_back(track_opt.value());
        }
    }
    resp_body["loaded_tracks"] = std::move(loaded_tracks_arr);

    // Собираем и отдаем JSON
    res.body() = resp_body.dump();
    res.prepare_payload();
    return res;
}
