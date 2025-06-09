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
 *   - "usertag1": string  — чей профиль (ID1, всегда)
 *   - "usertag2": string (optional) — чей профиль просматривается (ID2, если чужой)
 *
 * В ответе возвращает JSON с полями:
 * 1) user_info: {
 *       "username": string,
 *       "usertag": string,
 *       "followersnum": int,
 *       "followingnum": int,
 *       "tracksLoadedNum": int,
 *       "tracksAddedNum": int,
 *       // при usertag2:
 *       "isFollowing": bool,   // подписан ли usertag1 на usertag2
 *       "isFollowed": bool     // подписан ли usertag2 на usertag1
 *    }
 * 2) albums: ...
 * 3) liked_tracks: ...
 * 4) loaded_tracks: ...
 *
 * Логика:
 * - Если usertag2 передан, все структуры собираются для usertag2.
 * - Флаги приватности берутся у usertag2.
 */

class ProfileHandler {
public:
    ProfileHandler(const DB& db, std::string usertag1, std::optional<std::string> usertag2)
        : db_(db)
        , usertag1_(std::move(usertag1))
        , usertag2_(std::move(usertag2))
    {}

    // Формирует и возвращает HTTP‑ответ
    http::response<http::string_body> operator()(int http_version) const;

private:
    const DB& db_;
    std::string usertag1_;               // ID1, текущий
    std::optional<std::string> usertag2_; // ID2, профиль для просмотра
};