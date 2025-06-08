#include "search-handler.h"
#include <iostream>

json SearchHandler::handle_search(const std::string& query) const {
    json result;

    std::vector<std::string> usertags = db_.search_users(query);
    json users = json::array();
    for (const auto& usertag : usertags) {
        auto profile_opt = db_.get_user_profile_info(usertag);
        if (profile_opt) {
            json user_json;
            user_json["usertag"] = usertag;
            user_json["username"] = profile_opt->username;
            users.push_back(user_json);
        }
    }
    result["users"] = users;

    std::vector<int> album_ids = db_.search_albums(query);
    json albums = json::array();
    for (int album_id : album_ids) {
        auto album_opt = db_.get_album_info(album_id);
        if (album_opt) {
            json album_json = *album_opt;
            album_json["tracks"] = db_.get_tracks_for_album(album_id);
            albums.push_back(album_json);
        }
    }
    result["albums"] = albums;

    std::vector<int> track_ids = db_.search_tracks(query);
    json tracks = json::array();
    for (int track_id : track_ids) {
        auto track_opt = db_.get_track_info(track_id);
        if (track_opt) {
            tracks.push_back(*track_opt);
        }
    }
    result["tracks"] = tracks;

    return result;
}

http::response<http::string_body> SearchHandler::operator()(int http_version, const json& body) const {
    http::response<http::string_body> res{http::status::ok, http_version};
    res.set(http::field::content_type, "application/json");

    // Проверка, что есть query
    if (!body.contains("query") || !body["query"].is_string()) {
        res.result(http::status::bad_request);
        res.body() = R"({"status":"error","message":"Missing or invalid 'query'"})";
        res.prepare_payload();
        return res;
    }

    std::string query = body["query"].get<std::string>();

    try {
        json search_result = handle_search(query);

        json resp_body;
        resp_body["status"] = "ok";
        resp_body["results"] = std::move(search_result);

        res.body() = resp_body.dump();
    } catch (const std::exception& e) {
        res.result(http::status::internal_server_error);
        res.body() = std::string(R"({"status":"error","message":"Exception during search: )") + e.what() + "\"}";
    }

    res.prepare_payload();
    return res;
}
