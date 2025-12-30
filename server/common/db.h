#pragma once

#include <fstream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <boost/mysql.hpp>
#include <boost/mysql/tcp_ssl.hpp>
#include <boost/beast/core/detail/base64.hpp> 
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>
#include <optional>
#include <vector>
#include <cstdlib>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using json = nlohmann::json;

/**
 * Структура для базовой информации о пользователе (имя + флаги приватности).
 */
struct UserProfileInfo {
    std::string username;
    bool myLoadedTracks;   // флаг из users.myloadedtracks
    bool myTracks;         // флаг из users.mytracks
    bool myAlbums;         // флаг из users.myalbums
    bool myLoadedAlbums;   // флаг из users.myloadedalbums
};

class DB {
private:
    mutable boost::asio::io_context io_context_;
    mutable boost::asio::ssl::context ssl_ctx_;
    mutable boost::mysql::tcp_ssl_connection conn_;
    bool connected_;
    void log_error(const boost::mysql::error_code& ec, 
                   const boost::mysql::diagnostics& diag, 
                   const std::string& where) const;

public:
    DB(const std::string& host,
       const std::string& user,
       const std::string& password,
       const std::string& database);
    
    ~DB();
    
    bool is_connected() const;
    
    // Проверяем существование пользователя по username (для регистрации)
    bool user_exists_by_usertag(const std::string& usertag);
    
    std::optional<std::string> get_usertag_by_email(const std::string& email);

    // Проверяем существование пользователя по email (для входа)
    bool user_exists_by_email(const std::string& email);
    
    // Добавляем нового пользователя с username, email и password
    bool add_user(const std::string& username, const std::string& email, 
                  const std::string& password, const std::string& usertag);
    
    // Проверяем учетные данные по email и password (для входа)
    bool check_credentials_by_email(const std::string& email, 
                                    const std::string& password);

    std::vector<std::string> get_target_usertags_by_usertag(const std::string& subscriber_usertag) const;

    // Получить информацию об альбоме (или nullopt, если не найден)
    std::optional<json> get_album_info(int album_id) const;

    // Получить список треков для альбома (может вернуть пустой массив)
    json get_tracks_for_album(int album_id) const;
    
    std::vector<json> get_random_tracks(const std::vector<int>& exclude_ids, int limit) const; 
    // Получить информацию об отдельном треке (или nullopt, если не найден)
    std::optional<json> get_track_info(int track_id) const;

    std::vector<int> get_album_ids_by_usertag(const std::string& usertag) const;
    std::vector<int> get_track_ids_by_usertag(const std::string& usertag) const;

    // ------------------------- Дополнения для профиля -------------------------
    
    std::optional<std::string> get_user_blob_base64(const std::string& usertag) const;
    /**
     * Возвращает UserProfileInfo: username + три булевых флага приватности
     * (myloadedtracks, mytracks, myalbums) из таблицы users.
     */
    std::optional<UserProfileInfo> get_user_profile_info(const std::string& usertag) const;

    /**
     * Считает, сколько раз пользователь подписался (число строк WHERE subscriber_usertag = usertag).
     * Это «сколько человек я читаю».
     */
    int count_following(const std::string& usertag) const;

    bool is_following(const std::string& subscriber_usertag, const std::string& target_usertag) const;

    /**
     * Считает, сколько раз на пользователя подписались (число строк WHERE target_usertag = usertag).
     * Это «сколько человек подписано на меня».
     */
    int count_followers(const std::string& usertag) const;

    /**
     * Считает, сколько треков пользователь загрузил (число строк WHERE usertag = usertag в track_authors).
     */
    int count_tracks_loaded(const std::string& usertag) const;

    /**
     * Считает, сколько треков пользователь лайкнул (число строк WHERE usertag = usertag в track_likes).
     */
    int count_tracks_liked(const std::string& usertag) const;

    /**
     * Возвращает вектор всех track_id, которые пользователь лайкнул (из таблицы track_likes).
     */
    std::vector<int> get_liked_track_ids(const std::string& usertag) const;
    
    /**
     * Возвращает вектор всех album_id, которые пользователь лайкнул (из таблицы album_likes).
     */
    std::vector<int> get_liked_album_ids(const std::string& usertag) const;

    /**
     * Возвращает вектор всех track_id, которые пользователь загрузил (из таблицы track_authors).
     */
    std::vector<int> get_loaded_track_ids(const std::string& usertag) const;

    // =================== Методы для CREATE ===================
    // Создаёт новый альбом (без обложки) и возвращает его ID
    int create_empty_album(const std::string& title, const std::string& description = "");

    // Сохраняет BLOB обложки в только что созданном альбоме
    void store_album_cover(int album_id, const std::vector<uint8_t>& cover_blob);

    // Добавляет запись в таблицу album_authors
    void add_album_author(int album_id, const std::string& usertag);

    // Создаёт новый трек с привязкой к альбому и возвращает его ID
    int create_track(int album_id, const std::string& title, int duration_seconds, const std::vector<uint8_t>& file_blob, const std::vector<uint8_t>& cover_blob, const std::string& description = "", const std::string& track_text = "");

    // Добавляет запись в таблицу track_authors
    void add_track_author(int track_id, const std::string& usertag);
    
    // streaming blob
    std::optional<std::vector<uint8_t>> get_track_blob(int track_id) const;
    
    // album blob 
    std::optional<std::vector<uint8_t>> get_album_cover(int album_id) const;

    // track blob
    std::optional<std::vector<uint8_t>> get_track_cover(int track_id) const;

    std::optional<std::vector<uint8_t>> get_user_cover(const std::string& usertag) const;

    std::vector<std::string> search_users(const std::string& query) const;

    std::vector<int> search_albums(const std::string& query) const;
    
    std::vector<int> search_tracks(const std::string& query) const;

    // User actions

    std::vector<json> get_followers(const std::string& usertag) const;
    std::vector<json> get_following(const std::string& usertag) const;
    bool toggle_follow(const std::string& usertag1, const std::string& usertag2);

    bool toggle_like_track(const std::string& usertag, int track_id);
    bool toggle_like_album(const std::string& usertag, int album_id);

    bool is_album_liked(const std::string& usertag, int album_id) const;

    bool set_mytracks(const std::string& usertag, int flag);
    bool set_myloadedtracks(const std::string& usertag, int flag);
    bool set_myalbums(const std::string& usertag, int flag);
    bool set_myloadedalbums(const std::string& usertag, int flag);

    int save_profile_cover(const std::string& usertag, const std::vector<uint8_t>& profile_blob) const;

    bool set_default_profile_cover(const std::string& usertag) const;
};
