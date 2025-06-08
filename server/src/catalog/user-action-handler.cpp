#include "user-action-handler.h"

static std::vector<uint8_t> decode_base64(const std::string& b64) {
    size_t len = b64.size();
    size_t decoded_len = (len * 3) / 4;
    std::vector<uint8_t> out(decoded_len + 1);
    int out_len = EVP_DecodeBlock(out.data(),
                                  reinterpret_cast<const unsigned char*>(b64.data()),
                                  static_cast<int>(b64.size()));
    if (out_len < 0) {
        throw std::runtime_error("Base64 decode error");
    }
    out.resize(out_len);
    return out;
}

UserActionHandler::UserActionHandler(DB& db)
    : db_(db)
{
    // MyTracks: возвращаем полные структуры треков
    actions_["my_tracks"] = [this](const json& p) {
        std::vector<int> ids = db_.get_liked_track_ids(p["usertag"].get<std::string>());
        json arr = json::array();
        for (int id : ids) {
            if (auto opt = db_.get_track_info(id)) {
                arr.push_back(*opt);
            }
        }
        return json{{"tracks", arr}};
    };

    // MyAlbums: возвращаем полные структуры альбомов
    actions_["my_albums"] = [this](const json& p) {
        std::vector<int> ids = db_.get_liked_album_ids(p["usertag"].get<std::string>());
        json arr = json::array();
        for (int aid : ids) {
            if (auto opt = db_.get_album_info(aid)) {
                json album = *opt;
                album["tracks"] = db_.get_tracks_for_album(aid);
                arr.push_back(album);
            }
        }
        return json{{"albums", arr}};
    };

    // MyLoadedTracks: возвращаем загруженные треки
    actions_["my_loaded_tracks"] = [this](const json& p) {
        std::vector<int> ids = db_.get_loaded_track_ids(p["usertag"].get<std::string>());
        json arr = json::array();
        for (int id : ids) {
            if (auto opt = db_.get_track_info(id)) {
                arr.push_back(*opt);
            }
        }
        return json{{"tracks", arr}};
    };

    // Followers: структура пользователей + метрики
    actions_["followers"] = [this](const json& p) {
        auto raw = db_.get_followers(p["usertag"].get<std::string>());
        json arr = json::array();
        for (auto const& obj : raw) {
            std::string utag = obj["usertag"].get<std::string>();
            json u;
            u["usertag"] = utag;
            if (auto info = db_.get_user_profile_info(utag)) {
                u["username"] = info->username;
            }
            u["followersNum"]    = db_.count_followers(utag);
            u["followingNum"]    = db_.count_following(utag);
            u["tracksLoadedNum"] = db_.count_tracks_loaded(utag);
            u["tracksLikedNum"]  = db_.count_tracks_liked(utag);
            arr.push_back(u);
        }
        return json{{"users", arr}};
    };

    // Following: аналогично подпискам
    actions_["following"] = [this](const json& p) {
        auto raw = db_.get_following(p["usertag"].get<std::string>());
        json arr = json::array();
        for (auto const& obj : raw) {
            std::string utag = obj["usertag"].get<std::string>();
            json u;
            u["usertag"] = utag;
            if (auto info = db_.get_user_profile_info(utag)) {
                u["username"] = info->username;
            }
            u["followersNum"]    = db_.count_followers(utag);
            u["followingNum"]    = db_.count_following(utag);
            u["tracksLoadedNum"] = db_.count_tracks_loaded(utag);
            u["tracksLikedNum"]  = db_.count_tracks_liked(utag);
            arr.push_back(u);
        }
        return json{{"users", arr}};
    };

    actions_["follow"] = [this](const json& p) {
        bool now = db_.toggle_follow(
            p["usertag1"].get<std::string>(),
            p["usertag2"].get<std::string>()
        );
        return json{{"following", now}};
    };
    actions_["like_track"] = [this](const json& p) {
        bool now = db_.toggle_like_track(
            p["usertag"].get<std::string>(),
            p["track_id"].get<int>()
        );
        return json{{"liked", now}};
    };
    actions_["like_album"] = [this](const json& p) {
        bool now = db_.toggle_like_album(
            p["usertag"].get<std::string>(),
            p["album_id"].get<int>()
        );
        return json{{"liked", now}};
    };
    actions_["get_album"] = [this](const json& p) {
        int aid = p["album_id"].get<int>();
        json info = db_.get_album_info(aid).value_or(json{});
        info["tracks"] = db_.get_tracks_for_album(aid);
        info["liked_tracks"] = db_.get_liked_track_ids(p["usertag"].get<std::string>());
        info["album_liked"]  = db_.is_album_liked(p["usertag"].get<std::string>(), aid);
        return json{{"album", info}};
    };

    actions_["set_mytracks"] = [this](const json& p) {
        if (!p.contains("usertag") || !p.contains("flag"))
            throw std::runtime_error("Missing 'usertag' or 'flag' param");
        bool ok = db_.set_mytracks(p["usertag"].get<std::string>(), p["flag"].get<int>());
        return json{{"success", ok}};
    };

    actions_["set_myloadedtracks"] = [this](const json& p) {
        if (!p.contains("usertag") || !p.contains("flag"))
            throw std::runtime_error("Missing 'usertag' or 'flag' param");
        bool ok = db_.set_myloadedtracks(p["usertag"].get<std::string>(), p["flag"].get<int>());
        return json{{"success", ok}};
    };

    actions_["set_myalbums"] = [this](const json& p) {
        if (!p.contains("usertag") || !p.contains("flag"))
            throw std::runtime_error("Missing 'usertag' or 'flag' param");
        bool ok = db_.set_myalbums(p["usertag"].get<std::string>(), p["flag"].get<int>());
        return json{{"success", ok}};
    };

    actions_["set_myloadedalbums"] = [this](const json& p) {
        if (!p.contains("usertag") || !p.contains("flag"))
            throw std::runtime_error("Missing 'usertag' or 'flag' param");
        bool ok = db_.set_myloadedalbums(p["usertag"].get<std::string>(), p["flag"].get<int>());
        return json{{"success", ok}};
    };
    
    actions_["set_profile_cover"] = [this](const json& p) {
        if (!p.contains("usertag") || !p.contains("profile_cover"))
            throw std::runtime_error("Missing 'usertag' or 'profile_cover' param");

        const std::string usertag = p["usertag"].get<std::string>();
        const std::string cover_base64 = p["profile_cover"].get<std::string>();
        std::vector<uint8_t> profile_blob = decode_base64(cover_base64);

        int rc = db_.save_profile_cover(usertag, profile_blob);

        return json{{"success", rc == 0}};
    };
}

http::response<http::string_body> UserActionHandler::operator()(int http_version, const json& body) const {
    http::response<http::string_body> res{ http::status::ok, http_version };
    res.set(http::field::content_type, "application/json");

    if (!body.contains("subaction") || !body["subaction"].is_string()) {
        res.result(http::status::bad_request);
        res.body() = R"({"status":"error","message":"Missing or invalid 'subaction'"})";
        res.prepare_payload();
        return res;
    }
    std::string sub = body["subaction"].get<std::string>();
    json params  = body.value("params", json::object());

    auto it = actions_.find(sub);
    if (it == actions_.end()) {
        res.result(http::status::bad_request);
        res.body() = R"({"status":"error","message":"Unknown subaction"})";
        res.prepare_payload();
        return res;
    }

    json result;
    try {
        result = it->second(params);
    } catch (const std::exception& e) {
        res.result(http::status::internal_server_error);
        res.body() = std::string(R"({"status":"error","message":"Exception: )") + e.what() + "\"}";
        return res;
    }

    json resp_body;
    resp_body["status"] = "ok";
    resp_body.update(result);

    res.body() = resp_body.dump();
    res.prepare_payload();
    return res;
}
