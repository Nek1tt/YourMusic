#include "profile-handler.h"

static void log_mysql_error(const boost::mysql::error_code& ec,
                            const boost::mysql::diagnostics& diag,
                            const std::string& where)
{
    std::cerr << "[ProfileHandler] " << where << " error: " << ec.message() << "\n";
}

http::response<http::string_body> ProfileHandler::operator()(int http_version) const {
    http::response<http::string_body> res{ http::status::ok, http_version };
    res.set(http::field::content_type, "application/json");

    json resp_body;
    resp_body["status"] = "ok";

    // Выбираем, чей профиль показываем: usertag2 (чужой) или usertag1 (свой)
    const std::string& target = usertag2_.has_value() ? *usertag2_ : usertag1_;

    // Получаем базовую инфу о пользователе (username + флаги приватности)
    auto user_info_opt = db_.get_user_profile_info(target);
    if (!user_info_opt.has_value()) {
        resp_body = json{
            {"status", "error"},
            {"message", "User not found: " + target}
        };
        res.body() = resp_body.dump();
        res.prepare_payload();
        return res;
    }
    UserProfileInfo ui = user_info_opt.value();

    // Считаем подписки/подписчиков и треки для target
    int followersNum     = db_.count_followers(target);
    int followingNum     = db_.count_following(target);
    int tracksLoadedNum  = db_.count_tracks_loaded(target);
    int tracksAddedNum   = db_.count_tracks_liked(target);

    json user_info = {
        {"username", ui.username},
        {"usertag", target},
        {"followersnum", followersNum},
        {"followingnum", followingNum},
        {"tracksLoadedNum", tracksLoadedNum},
        {"tracksAddedNum", tracksAddedNum}
    };

    // При просмотре чужого профиля — проверяем взаимные подписки
    if (usertag2_.has_value()) {
        bool isFollowing = db_.is_following(usertag1_, target);
        bool isFollowed  = db_.is_following(target, usertag1_);
        user_info["isFollowing"] = isFollowing;
        user_info["isFollowed"]  = isFollowed;
    }
    resp_body["user_info"] = std::move(user_info);

    // Определяем, показывать ли всю информацию (свой профиль) или применять флаги (чужой профиль)
    bool is_own_profile = (target == usertag1_);

    // 3) Альбомы (загруженные)
    json albums_arr = json::array();
    if (is_own_profile || ui.myLoadedAlbums) {
        auto album_ids = db_.get_album_ids_by_usertag(target);
        for (int album_id : album_ids) {
            auto album_opt = db_.get_album_info(album_id);
            if (!album_opt) continue;
            json a = album_opt.value();
            json album_obj = {
                {"id", a["id"]},
                {"description", a["description"]},
                {"title", a["title"]},
                {"authors", a["authors"]},
                {"author_usertag",  a["author_usertag"]},
                {"created_date", a["created_date"]},
                {"total_duration", a["total_duration"]},
                {"track_count", a["track_count"]},
                {"tracks", db_.get_tracks_for_album(album_id)}
            };
            albums_arr.push_back(std::move(album_obj));
        }
    }
    resp_body["loaded_albums"] = std::move(albums_arr);

    // Лайкнутые альбомы
    json liked_albums_arr = json::array();
    if (is_own_profile || ui.myAlbums) {
        auto liked_ids = db_.get_liked_album_ids(target);
        for (int album_id : liked_ids) {
            if (auto album_opt = db_.get_album_info(album_id)) {
                json a = album_opt.value();
                json album_obj = {
                    {"id", a["id"]},
                    {"description", a["description"]},
                    {"title", a["title"]},
                    {"authors", a["authors"]},
                    {"author_usertag",  a["author_usertag"]},
                    {"created_date", a["created_date"]},
                    {"total_duration", a["total_duration"]},
                    {"track_count", a["track_count"]},
                    {"tracks", db_.get_tracks_for_album(album_id)}
                };
                liked_albums_arr.push_back(std::move(album_obj));
            }
        }
    }
    resp_body["liked_albums"] = std::move(liked_albums_arr);

    // Лайкнутые треки
    json liked_tracks_arr = json::array();
    if (is_own_profile || ui.myTracks) {
        for (int tid : db_.get_liked_track_ids(target)) {
            auto topt = db_.get_track_info(tid);
            if (!topt) continue;
            json t = topt.value();
            liked_tracks_arr.push_back(std::move(t));
        }
    }
    resp_body["liked_tracks"] = std::move(liked_tracks_arr);

    // Загруженные треки
    json loaded_tracks_arr = json::array();
    if (is_own_profile || ui.myLoadedTracks) {
        for (int tid : db_.get_loaded_track_ids(target)) {
            auto topt = db_.get_track_info(tid);
            if (!topt) continue;
            json t = topt.value();
            loaded_tracks_arr.push_back(std::move(t));
        }
    }
    resp_body["loaded_tracks"] = std::move(loaded_tracks_arr);

    res.body() = resp_body.dump();
    res.prepare_payload();
    return res;
}