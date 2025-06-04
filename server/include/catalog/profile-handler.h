#pragma once

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include "db.h"
#include <boost/mysql.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http  = beast::http;
using json = nlohmann::json;

/**
 * Хендлер “PROFILE”.
 *
 * На вход принимает JSON с:
 *   - "usertag": string  — чей профиль запрашиваем
 *   - "flag": int        — 0 = свой профиль, 1 = чужой профиль
 *
 * В ответе возвращает JSON с полями:
 * 1) user_info: {
 *       "username": string,
 *       "usertag": string,
 *       "followersnum": int,
 *       "followingnum": int,
 *       "tracksLoadedNum": int,
 *       "tracksAddedNum": int
 *    }
 * 2) albums: [ { album_id, title, author_username, author_usertag, track_count, "tracks": [ ... ] }, ... ]
 * 3) liked_tracks: [ { /* структура get_track_info()  }, ... ]
 * 4) loaded_tracks: [ { /* структура get_track_info()  }, ... ]
 *
 * Если flag=1 (чужой профиль), то проверяются флаги приватности 
 * из users.(myalbums, mytracks, myloadedtracks). При значении 0 соответствующий 
 * массив возвращается как пустой.
 */

class ProfileHandler {
public:
    ProfileHandler(const DB& db, std::string usertag, int flag)
        : db_(db), usertag_(std::move(usertag)), flag_(flag) {}

    // Формирует и возвращает HTTP‑ответ
    http::response<http::string_body> operator()(int http_version) const;

private:
    const DB& db_;
    std::string usertag_;
    int flag_;  // 0 = свой, 1 = чужой
};
