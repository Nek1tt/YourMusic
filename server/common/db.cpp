#include "db.h"

DB::DB(const std::string& host,
       const std::string& user,
       const std::string& password,
       const std::string& database)
    : ssl_ctx_(net::ssl::context::tls_client),
      conn_(io_context_, ssl_ctx_),
      connected_(false)
{
    try {
        net::ip::tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, "3306");

        boost::mysql::handshake_params params(user, password, database);
        boost::mysql::error_code ec;
        boost::mysql::diagnostics diag;

        conn_.connect(*endpoints.begin(), params, ec, diag);
        if (ec) {
            std::cerr << "[DB] Connection error: " << ec.message() << "\n";
        } else {
            connected_ = true;
        }
    } catch (const std::exception& e) {
        std::cerr << "[DB] Exception during init: " << e.what() << "\n";
    }
}

DB::~DB() {
    if (connected_) {
        boost::mysql::error_code ec;
        boost::mysql::diagnostics diag;
        conn_.close(ec, diag);
    }
}

bool DB::is_connected() const {
    return connected_;
}

void DB::log_error(const boost::mysql::error_code& ec, 
                   const boost::mysql::diagnostics& diag, 
                   const std::string& where) const {
    std::cerr << "[DB] " << where << " error: " << ec.message() << "\n";
}

static std::string blob_to_hex(const std::vector<uint8_t>& blob) {
    static const char* hex_digits = "0123456789ABCDEF";
    std::string out;
    out.reserve(blob.size() * 2);
    for (uint8_t byte : blob) {
        out += hex_digits[(byte >> 4) & 0xF];
        out += hex_digits[byte & 0xF];
    }
    return out;
}

bool DB::user_exists_by_usertag(const std::string& usertag)
{
    if (!connected_) return false;

    std::string query = "SELECT username FROM users WHERE usertag = '" + usertag + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "user_exists_by_usertag execute error: " << ec.message() << "\n";
        return false;
    }
    return !result.rows().empty();
}

std::optional<std::string> DB::get_usertag_by_email(const std::string& email)
{
    if (!connected_) return std::nullopt;

    std::string query = "SELECT usertag FROM users WHERE email = '" + email + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "get_usertag_by_email execute error: " << ec.message() << "\n";
        return std::nullopt;
    }

    auto rows = result.rows();
    if (rows.empty()) {
        return std::nullopt;
    }
    return rows.at(0).at(0).as_string();
}

// Проверяем, существует ли пользователь с данным email (для входа)
bool DB::user_exists_by_email(const std::string& email)
{
    if (!connected_) return false;

    std::string query = "SELECT usertag FROM users WHERE email = '" + email + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "user_exists_by_email execute error: " << ec.message() << "\n";
        return false;
    }
    return !result.rows().empty();
}

// Добавляем нового пользователя (username, email, password)
bool DB::add_user(const std::string& username, 
                  const std::string& email, 
                  const std::string& password, 
                  const std::string& usertag)
{
    if (!connected_) return false;

    std::string query = 
        "INSERT INTO users (username, email, password, usertag) VALUES ('" 
        + username + "', '" 
        + email + "', '"
        + password + "', '"
        + usertag + "')";
    boost::mysql::results result;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "add_user execute error: " << ec.message() << "\n";
        return false;
    }
    return true;
}

// Проверяем учетные данные по email и password (для входа)
bool DB::check_credentials_by_email(const std::string& email, 
                                    const std::string& password)
{
    if (!connected_) return false;

    std::string query = 
        "SELECT usertag FROM users WHERE email = '" + email 
        + "' AND password = '" + password + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "check_credentials_by_email execute error: " << ec.message() << "\n";
        return false;
    }
    return !result.rows().empty();
}

std::vector<std::string> DB::get_target_usertags_by_usertag(const std::string& subscriber_usertag) const {
    std::cout << "[DB] get_target_usertags_by_usertag() called with subscriber_usertag = '"
              << subscriber_usertag << "'\n";

    std::vector<std::string> result_tags;
    if (!connected_) {
        std::cerr << "[DB] get_target_usertags_by_usertag() error: not connected to database\n";
        return result_tags;
    }

    std::string query =
        "SELECT DISTINCT target_usertag "
        "FROM subscriptions "
        "WHERE subscriber_usertag = '" + subscriber_usertag + "'";
    std::cout << "[DB] Executing SQL: " << query << "\n";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results rows;
    conn_.execute(query, rows, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_target_usertags_by_usertag() SQL error: " << ec.message() << "\n";
        return result_tags;
    }

    for (auto const& row : rows.rows()) {
        if (!row.at(0).is_null()) {
            std::string tag = row.at(0).as_string();
            result_tags.push_back(tag);
            std::cout << "[DB]   found target_usertag = '" << tag << "'\n";
        }
    }

    std::cout << "[DB] get_target_usertags_by_usertag() returning "
              << result_tags.size() << " tag(s)\n";
    return result_tags;
}

std::optional<json> DB::get_album_info(int album_id) const {
    std::cout << "[DB] get_album_info() called with album_id = "
              << album_id << "\n";

    if (!connected_) {
        std::cerr << "[DB] get_album_info() error: not connected to database\n";
        return std::nullopt;
    }

    std::string query =
    "SELECT "
    "  a.album_id AS id, "
    "  a.title, "
    "  a.description, "
    "  (SELECT GROUP_CONCAT(u.username) "
    "     FROM album_authors aa "
    "     JOIN users u ON u.usertag = aa.usertag "
    "    WHERE aa.album_id = a.album_id) AS authors, "
    "  (SELECT aa.usertag "
    "     FROM album_authors aa "
    "    WHERE aa.album_id = a.album_id "
    "    LIMIT 1) AS author_usertag, "
    "  (SELECT COUNT(*) FROM tracks t WHERE t.album_id = a.album_id) AS track_count, "
    "  a.release_date AS created_date, "
    "  CAST( COALESCE( (SELECT SUM(duration_seconds) "
    "                   FROM tracks t WHERE t.album_id = a.album_id), 0 ) "
    "       AS UNSIGNED) AS total_duration "
    "FROM albums a "
    "WHERE a.album_id = " + std::to_string(album_id);

    std::cout << "[DB] Executing SQL: " << query << "\n";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results results;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_album_info() SQL error: " << ec.message() << "\n";
        return std::nullopt;
    }
    if (results.rows().empty()) {
        std::cout << "[DB] get_album_info() no rows found for album_id = "
                  << album_id << "\n";
        return std::nullopt;
    }

    auto row = *results.rows().begin();
    json album;
    album["id"] = static_cast<int>(row.at(0).as_int64());
    album["title"] = std::string{row.at(1).as_string()};
    album["description"] = row.at(2).is_null() ? "" : std::string{row.at(2).as_string()};
    album["authors"] = row.at(3).is_null() ? "" : std::string{row.at(3).as_string()};
    album["author_usertag"] = row.at(4).is_null() ? "" : std::string{row.at(4).as_string()};
    album["track_count"]   = static_cast<int>(row.at(5).as_int64());

    auto d = row.at(6).as_date();
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << static_cast<int>(d.year()) << '-'
        << std::setw(2) << std::setfill('0') << static_cast<int>(d.month()) << '-'
        << std::setw(2) << std::setfill('0') << static_cast<int>(d.day());
    album["created_date"]   = oss.str();
    album["total_duration"] = static_cast<int>(row.at(7).as_uint64());

    std::cout << "[DB] get_album_info() fetched: "
              << " title = \"" << album["title"].get<std::string>() << "\","
              << " authors = \"" << album["authors"].get<std::string>() << "\","
              << " track_count = " << album["track_count"].get<int>() << ","
              << " created_date = " << album["created_date"].get<std::string>() << ","
              << " total_duration = " << album["total_duration"].get<int>() << "\n";

    return album;
}

json DB::get_tracks_for_album(int album_id) const {
    std::cout << "[DB] get_tracks_for_album() called with album_id = "
              << album_id << "\n";

    if (!connected_) {
        std::cerr << "[DB] get_tracks_for_album() error: not connected to database\n";
        return json::array();
    }

    std::string query =
        "SELECT "
        "  t.track_id AS id, "
        "  t.title, "
        "  t.album_id, " 
        "  t.upload_date, "
        "  (SELECT GROUP_CONCAT(u.username) "
        "     FROM track_authors ta "
        "     JOIN users u ON u.usertag = ta.usertag "
        "    WHERE ta.track_id = t.track_id) AS authors, "
        "  t.duration_seconds "
        "FROM tracks t "
        "JOIN albums a ON a.album_id = t.album_id "
        "WHERE t.album_id = " + std::to_string(album_id);

    std::cout << "[DB] Executing SQL: " << query << "\n";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results results;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_tracks_for_album() SQL error: " << ec.message() << "\n";
        return json::array();
    }

    json arr = json::array();
    for (auto const& row : results.rows()) {
        json track;
        int id = static_cast<int>(row.at(0).as_int64());
        std::string title   = std::string{row.at(1).as_string()};
        int aid             = static_cast<int>(row.at(2).as_int64());
        auto d              = row.at(3).as_date();
        std::ostringstream oss;
        oss << std::setw(4) << std::setfill('0') << static_cast<int>(d.year()) << '-'
            << std::setw(2) << std::setfill('0') << static_cast<int>(d.month()) << '-'
            << std::setw(2) << std::setfill('0') << static_cast<int>(d.day());

        track["id"]               = id;
        track["title"]            = title;
        track["album_id"]         = aid;
        track["upload_date"]      = oss.str();
        track["authors"]          = row.at(4).is_null() ? "" : std::string{row.at(4).as_string()};
        track["duration_seconds"] = static_cast<int>(row.at(5).as_int64());

        std::cout << "[DB]   fetched track: id=" << id
                  << ", title=\"" << title << "\""
                  << ", upload_date=" << track["upload_date"].get<std::string>()
                  << ", duration_seconds=" << track["duration_seconds"].get<int>() << "\n";

        arr.push_back(track);
    }

    std::cout << "[DB] get_tracks_for_album() returning " << arr.size() << " track(s)\n";
    return arr;
}

std::optional<json> DB::get_track_info(int track_id) const {
    std::cout << "[DB] get_track_info() called with track_id = "
              << track_id << "\n";

    if (!connected_) {
        std::cerr << "[DB] get_track_info() error: not connected to database\n";
        return std::nullopt;
    }

    std::string query =
        "SELECT "
        "  t.track_id AS id, "
        "  t.title, "
        "  t.album_id, "
        "  t.upload_date, "
        "  t.description, "
        "  t.track_text,  "
        "  (SELECT GROUP_CONCAT(u.username) "
        "     FROM track_authors ta "
        "     JOIN users u ON u.usertag = ta.usertag "
        "    WHERE ta.track_id = t.track_id) AS authors, "
        "  (SELECT ta.usertag "
        "     FROM track_authors ta "
        "    WHERE ta.track_id = t.track_id "
        "    LIMIT 1) AS author_usertag, "
        "  t.duration_seconds "
        "FROM tracks t "
        "LEFT JOIN albums a ON a.album_id = t.album_id "
        "WHERE t.track_id = " + std::to_string(track_id);

    std::cout << "[DB] Executing SQL: " << query << "\n";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results results;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_track_info() SQL error: " << ec.message() << "\n";
        return std::nullopt;
    }

    if (results.rows().empty()) {
        std::cout << "[DB] get_track_info() no rows found for track_id = "
                  << track_id << "\n";
        return std::nullopt;
    }

    auto row = *results.rows().begin();
    json track;
    track["id"] = static_cast<int>(row.at(0).as_int64());
    track["title"] = std::string{row.at(1).as_string()};
    int album_id = row.at(2).is_null() ? 0 : static_cast<int>(row.at(2).as_int64());
    track["album_id"] = album_id;
    auto d = row.at(3).as_date();
    std::ostringstream date_ss;
    date_ss << std::setw(4) << std::setfill('0') << static_cast<int>(d.year()) << '-'
            << std::setw(2) << std::setfill('0') << static_cast<int>(d.month()) << '-'
            << std::setw(2) << std::setfill('0') << static_cast<int>(d.day());
    track["upload_date"] = date_ss.str();

    if (row.at(4).is_null())
        track["description"] = "";
    else
        track["description"] = std::string{row.at(4).as_string()};

    track["track_text"] = row.at(5).is_null() ? "" : std::string{row.at(5).as_string()};

    track["authors"] = row.at(6).is_null() ? "" : std::string{row.at(6).as_string()};

    track["author_usertag"] = row.at(7).is_null() ? "" : std::string{row.at(7).as_string()};

    track["duration_seconds"] = static_cast<int>(row.at(8).as_int64());

    std::cout << "[DB] get_track_info() fetched: "
              << "id=" << track["id"].get<int>()
              << ", title=\"" << track["title"].get<std::string>() << "\""
              << ", author_usertag=\"" << track["author_usertag"].get<std::string>() << "\""
              << ", duration_seconds=" << track["duration_seconds"].get<int>() << "\n";

    return track;
}


std::vector<int> DB::get_album_ids_by_usertag(const std::string& usertag) const {
    std::cout << "[DB] get_album_ids_by_usertag() called with usertag = '"
              << usertag << "'\n";

    std::vector<int> album_ids;
    if (!connected_) {
        std::cerr << "[DB] get_album_ids_by_usertag() error: not connected to database\n";
        return album_ids;
    }

    std::string query =
        "SELECT DISTINCT album_id "
        "FROM album_authors "
        "WHERE usertag = '" + usertag + "'";
    std::cout << "[DB] Executing SQL: " << query << "\n";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results rows;
    conn_.execute(query, rows, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_album_ids_by_usertag() SQL error: " << ec.message() << "\n";
        return album_ids;
    }

    for (auto const& row : rows.rows()) {
        int id = static_cast<int>(row.at(0).as_int64());
        album_ids.push_back(id);
        std::cout << "[DB]   found album_id = " << id << "\n";
    }

    std::cout << "[DB] get_album_ids_by_usertag() returning "
              << album_ids.size() << " id(s)\n";
    return album_ids;
}

std::vector<int> DB::get_track_ids_by_usertag(const std::string& usertag) const {
    std::cout << "[DB] get_track_ids_by_usertag() called with usertag = '"
              << usertag << "'\n";

    std::vector<int> track_ids;
    if (!connected_) {
        std::cerr << "[DB] get_track_ids_by_usertag() error: not connected to database\n";
        return track_ids;
    }

    std::string query =
        "SELECT DISTINCT track_id "
        "FROM track_authors "
        "WHERE usertag = '" + usertag + "'";
    std::cout << "[DB] Executing SQL: " << query << "\n";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results rows;
    conn_.execute(query, rows, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_track_ids_by_usertag() SQL error: " << ec.message() << "\n";
        return track_ids;
    }

    for (auto const& row : rows.rows()) {
        int id = static_cast<int>(row.at(0).as_int64());
        track_ids.push_back(id);
        std::cout << "[DB]   found track_id = " << id << "\n";
    }

    std::cout << "[DB] get_track_ids_by_usertag() returning "
              << track_ids.size() << " id(s)\n";
    return track_ids;
}

std::optional<std::string> DB::get_user_blob_base64(const std::string& usertag) const {
    if (!connected_) {
        std::cerr << "[DB] get_user_blob_base64() error: not connected to database\n";
        return std::nullopt;
    }

    std::string query = "SELECT cover_blob FROM users WHERE usertag = '" + usertag + "'";

    boost::mysql::results results;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;

    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_user_blob_base64() SQL error: " << ec.message() << "\n";
        return std::nullopt;
    }

    if (results.rows().empty() || results.rows()[0].at(0).is_null()) {
        std::cout << "[DB] get_user_blob_base64() no cover_blob for usertag=" << usertag << "\n";
        return std::nullopt;
    }

    auto blob = results.rows()[0].at(0).as_blob();

    std::string encoded;
    encoded.reserve(boost::beast::detail::base64::encoded_size(blob.size()));
    boost::beast::detail::base64::encode(&encoded, blob.data(), blob.size());

    std::cout << "[DB] get_user_blob_base64() found cover_blob for usertag=" << usertag
              << ", size=" << blob.size() << " → base64 size=" << encoded.size() << "\n";

    return encoded;
}

std::optional<UserProfileInfo> DB::get_user_profile_info(const std::string& usertag) const {
    if (!connected_) return std::nullopt;

    std::string query =
        "SELECT username, myloadedtracks, mytracks, myalbums, myloadedalbums "
        "FROM users "
        "WHERE usertag = '" + usertag + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        log_error(ec, diag, "get_user_profile_info");
        return std::nullopt;
    }
    if (result.rows().empty()) {
        return std::nullopt;
    }
    auto row = *result.rows().begin();
    UserProfileInfo info;
    info.username        = std::string{ row.at(0).as_string() };
    info.myLoadedTracks = (row.at(1).as_int64() != 0);
    info.myTracks        = (row.at(2).as_int64() != 0);
    info.myAlbums        = (row.at(3).as_int64() != 0);
    info.myLoadedAlbums = (row.at(4).as_int64() != 0);
    return info;
}

int DB::count_following(const std::string& usertag) const {
    if (!connected_) return 0;

    std::string query =
        "SELECT COUNT(*) "
        "FROM subscriptions "
        "WHERE subscriber_usertag = '" + usertag + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        log_error(ec, diag, "count_following");
        return 0;
    }
    auto row = *result.rows().begin();
    return static_cast<int>(row.at(0).as_int64());
}

int DB::count_followers(const std::string& usertag) const {
    if (!connected_) return 0;

    std::string query =
        "SELECT COUNT(*) "
        "FROM subscriptions "
        "WHERE target_usertag = '" + usertag + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        log_error(ec, diag, "count_followers");
        return 0;
    }
    auto row = *result.rows().begin();
    return static_cast<int>(row.at(0).as_int64());
}

int DB::count_tracks_loaded(const std::string& usertag) const {
    if (!connected_) return 0;

    std::string query =
        "SELECT COUNT(*) "
        "FROM track_authors "
        "WHERE usertag = '" + usertag + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        log_error(ec, diag, "count_tracks_loaded");
        return 0;
    }
    auto row = *result.rows().begin();
    return static_cast<int>(row.at(0).as_int64());
}

int DB::count_tracks_liked(const std::string& usertag) const {
    if (!connected_) return 0;

    std::string query =
        "SELECT COUNT(*) "
        "FROM track_likes "
        "WHERE usertag = '" + usertag + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        log_error(ec, diag, "count_tracks_liked");
        return 0;
    }
    auto row = *result.rows().begin();
    return static_cast<int>(row.at(0).as_int64());
}

std::vector<int> DB::get_liked_track_ids(const std::string& usertag) const {
    std::cout << "[DB] get_liked_track_ids() called with usertag = '"
              << usertag << "'\n";

    std::vector<int> ids;
    if (!connected_) {
        std::cerr << "[DB] get_liked_track_ids() error: not connected to database\n";
        return ids;
    }

    std::string query =
        "SELECT track_id "
        "FROM track_likes "
        "WHERE usertag = '" + usertag + "'";
    std::cout << "[DB] Executing SQL: " << query << "\n";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results rows;
    conn_.execute(query, rows, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_liked_track_ids() SQL error: " << ec.message() << "\n";
        return ids;
    }

    for (auto const& row : rows.rows()) {
        int id = static_cast<int>(row.at(0).as_int64());
        ids.push_back(id);
        std::cout << "[DB]   found liked track_id = " << id << "\n";
    }

    std::cout << "[DB] get_liked_track_ids() returning "
              << ids.size() << " id(s)\n";
    return ids;
}

std::vector<int> DB::get_liked_album_ids(const std::string& usertag) const {
    std::vector<int> album_ids;
    if (!connected_) return album_ids;

    std::string query =
        "SELECT album_id "
        "FROM album_likes "
        "WHERE usertag = '" + usertag + "'";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results rows;
    conn_.execute(query, rows, ec, diag);
    if (ec) {
        log_error(ec, diag, "get_liked_album_ids");
        return album_ids;
    }

    for (auto const& row : rows.rows()) {
        album_ids.push_back(static_cast<int>(row.at(0).as_int64()));
    }
    return album_ids;
}

std::vector<int> DB::get_loaded_track_ids(const std::string& usertag) const {
    std::vector<int> ids;
    if (!connected_) return ids;

    std::string query =
        "SELECT track_id "
        "FROM track_authors "
        "WHERE usertag = '" + usertag + "'";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results rows;
    conn_.execute(query, rows, ec, diag);
    if (ec) {
        log_error(ec, diag, "get_loaded_track_ids");
        return ids;
    }
    for (auto const& row : rows.rows()) {
        ids.push_back(static_cast<int>(row.at(0).as_int64()));
    }
    return ids;
}

int DB::create_empty_album(const std::string& title, const std::string& description)
{
    if (!connected_) {
        return -1;
    }

    std::string q =
        "INSERT INTO albums (title, description, release_date) VALUES ('"
        + title + "', '"
        + description  + "', NOW())";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results r1;
    conn_.execute(q, r1, ec, diag);
    if (ec) {
        log_error(ec, diag, "create_empty_album");
        return -1;
    }

    uint64_t rawId = r1.last_insert_id();
    int newId = static_cast<int>(rawId);
    if (newId <= 0) {
        return -1;
    }
    return newId;
}

void DB::store_album_cover(int album_id, const std::vector<uint8_t>& cover_blob)
{
    if (!connected_) return;
    std::string hex = blob_to_hex(cover_blob);
    std::string q = 
        "UPDATE albums SET cover_blob = x'" + hex + "' WHERE album_id = " 
        + std::to_string(album_id);
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results r;
    conn_.execute(q, r, ec, diag);
    if (ec) {
        log_error(ec, diag, "store_album_cover");
    }
}

void DB::add_album_author(int album_id, const std::string& usertag)
{
    if (!connected_) return;

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;

    boost::mysql::statement stmt = conn_.prepare_statement(
        "INSERT INTO album_authors (album_id, usertag) VALUES (?, ?)",
        ec, diag
    );
    if (ec) {
        std::cerr << "[DB] error prepare_statement: " << ec.message() << "\n";
        return;
    }

    auto bound_stmt = stmt.bind(album_id, usertag);
    boost::mysql::results r;
    conn_.execute(bound_stmt, r, ec, diag);
    if (ec) {
        std::cerr << "[DB] error execute: " << ec.message() << "\n";
    }
}

int DB::create_track(int album_id,const std::string& title, int duration_seconds, const std::vector<uint8_t>& file_blob, const std::vector<uint8_t>& cover_blob, const std::string& description, const std::string& track_text)
{
    if (!connected_) return -1;

    std::string hexFile  = blob_to_hex(file_blob);
    std::string hexCover = blob_to_hex(cover_blob);

    std::string album_part = (album_id > 0 ? std::to_string(album_id) : "NULL");

    std::string q =
        "INSERT INTO tracks "
        "(title, album_id, duration_seconds, file_blob, cover_blob, upload_date, description, track_text) "
        "VALUES ('" + title  + "', "
                 + album_part     + ", "
                 + std::to_string(duration_seconds) + ", "
        "x'" + hexFile + "', "
        "x'" + hexCover + "', "
        "NOW(), '"
        + description + "', '"
        + track_text + "')";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results r1;
    conn_.execute(q, r1, ec, diag);
    if (ec) {
        log_error(ec, diag, "create_track");
        return -1;
    }

    int newTrackId = static_cast<int>(r1.last_insert_id());
    return newTrackId;
}

void DB::add_track_author(int track_id, const std::string& usertag)
{
    if (!connected_) return;
    std::string q = 
        "INSERT INTO track_authors (track_id, usertag) VALUES (" 
        + std::to_string(track_id) + ", '" 
        + usertag + "')";
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results r;
    conn_.execute(q, r, ec, diag);
    if (ec) {
        log_error(ec, diag, "add_track_author");
    }
}

std::vector<json> DB::get_random_tracks(const std::vector<int>& exclude_ids, int limit) const {
    std::vector<json> random_list;
    if (!connected_) {
        std::cerr << "[DB] get_random_tracks() error: not connected to database\n";
        return random_list;
    }

    std::string not_in = "0";
    if (!exclude_ids.empty()) {
        std::ostringstream oss;
        for (size_t i = 0; i < exclude_ids.size(); ++i) {
            if (i) oss << ",";
            oss << exclude_ids[i];
        }
        not_in = oss.str();
    }

    std::ostringstream sql;
    sql << "SELECT "
           "t.track_id AS id, "
           "t.title, "
           "t.description, "
           "t.album_id, "
           "t.upload_date, "
           "(SELECT GROUP_CONCAT(u.username) "
             "FROM track_authors ta "
             "JOIN users u ON u.usertag = ta.usertag "
            "WHERE ta.track_id = t.track_id) AS authors, "
           "(SELECT ta.usertag "
             "FROM track_authors ta "
            "WHERE ta.track_id = t.track_id "
            "LIMIT 1) AS author_usertag, "
           "t.duration_seconds "
        "FROM tracks t "
        "WHERE t.album_id IS NULL "
        " AND t.track_id NOT IN (" << not_in << ") "
        "ORDER BY RAND() "
        "LIMIT " << limit;

    std::string query = sql.str();
    std::cout << "[DB] get_random_tracks() SQL:\n" << query << "\n";

    boost::mysql::results results;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_random_tracks() SQL error: " << ec.message() << "\n";
        return random_list;
    }

    for (auto const& row : results.rows()) {
        int id                = static_cast<int>(row.at(0).as_int64());
        std::string title     = std::string{row.at(1).as_string()};
        std::string desc      = row.at(2).is_null() ? "" : std::string{row.at(2).as_string()};

        auto d = row.at(4).as_date();
        std::ostringstream ds;
        ds << std::setw(4) << std::setfill('0') << static_cast<int>(d.year()) << '-'
           << std::setw(2) << std::setfill('0') << static_cast<int>(d.month()) << '-'
           << std::setw(2) << std::setfill('0') << static_cast<int>(d.day());
        std::string upload_date = ds.str();

        std::string authors = row.at(5).is_null() ? "" : std::string{row.at(5).as_string()};
        std::string author_usertag = row.at(6).is_null() ? "" : std::string{row.at(6).as_string()};
        int duration = static_cast<int>(row.at(7).as_int64());

        json obj = {
            {"id", id},
            {"title", title},
            {"description", desc},
            {"album_id", 0},
            {"upload_date", upload_date},
            {"authors", authors},
            {"author_usertag", author_usertag},
            {"duration_seconds", duration},
            {"random", true}
        };

        std::cout << "[DB]   get_random_tracks() found: id=" << id
                  << ", author_usertag=" << author_usertag
                  << ", duration=" << duration << "\n";

        random_list.push_back(std::move(obj));
    }

    return random_list;
}

std::optional<std::vector<uint8_t>> DB::get_track_blob(int track_id) const {
    if (!connected_) {
        std::cerr << "[DB] get_track_blob: not connected to database\n";
        return std::nullopt;
    }

    std::string query =
        "SELECT file_blob "
        "FROM tracks "
        "WHERE track_id = " + std::to_string(track_id);

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results results;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_track_blob() SQL error: " << ec.message() << "\n";
        return std::nullopt;
    }
    if (results.rows().empty()) {
        std::cerr << "[DB] get_track_blob() no row for track_id = " << track_id << "\n";
        return std::nullopt;
    }

    auto row = *results.rows().begin();
    if (row.at(0).is_null()) {
        std::cerr << "[DB] get_track_blob() blob is NULL for track_id = " << track_id << "\n";
        return std::nullopt;
    }

    boost::mysql::blob_view blob_view = row.at(0).as_blob();

    std::vector<uint8_t> blob;
    blob.resize(blob_view.size());
    std::memcpy(blob.data(), blob_view.data(), blob_view.size());

    std::cout << "[DB] get_track_blob() fetched " << blob.size()
              << " bytes for track_id = " << track_id << "\n";

    return blob;
}

std::optional<std::vector<uint8_t>> DB::get_album_cover(int album_id) const {
    if (!connected_) {
        std::cerr << "[DB] get_album_blob: not connected to database\n";
        return std::nullopt;
    }

    std::string query =
        "SELECT cover_blob "
        "FROM albums "
        "WHERE album_id = " + std::to_string(album_id);

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results results;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_album_blob() SQL error: " << ec.message() << "\n";
        return std::nullopt;
    }
    if (results.rows().empty()) {
        std::cerr << "[DB] get_album_blob() no row for album_id = " << album_id << "\n";
        return std::nullopt;
    }

    auto row = *results.rows().begin();
    if (row.at(0).is_null()) {
        std::cerr << "[DB] get_album_blob() blob is NULL for album_id = " << album_id << "\n";
        return std::nullopt;
    }

    boost::mysql::blob_view blob_view = row.at(0).as_blob();

    std::vector<uint8_t> blob;
    blob.resize(blob_view.size());
    std::memcpy(blob.data(), blob_view.data(), blob_view.size());

    std::cout << "[DB] get_album_blob() fetched " << blob.size()
              << " bytes for album_id = " << album_id << "\n";

    return blob;
}

std::optional<std::vector<uint8_t>> DB::get_track_cover(int track_id) const {
    if (!connected_) {
        std::cerr << "[DB] get_track_cover_blob: not connected to database\n";
        return std::nullopt;
    }

    std::string query =
        "SELECT cover_blob "
        "FROM tracks "
        "WHERE track_id = " + std::to_string(track_id);

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results results;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_track_cover_blob() SQL error: " << ec.message() << "\n";
        return std::nullopt;
    }
    if (results.rows().empty()) {
        std::cerr << "[DB] get_track_cover_blob() no row for track_id = " << track_id << "\n";
        return std::nullopt;
    }

    auto row = *results.rows().begin();
    if (row.at(0).is_null()) {
        std::cerr << "[DB] get_track_cover_blob() blob is NULL for track_id = " << track_id << "\n";
        return std::nullopt;
    }

    boost::mysql::blob_view blob_view = row.at(0).as_blob();

    std::vector<uint8_t> blob;
    blob.resize(blob_view.size());
    std::memcpy(blob.data(), blob_view.data(), blob_view.size());

    std::cout << "[DB] get_track_cover_blob() fetched " << blob.size()
              << " bytes for track_id = " << track_id << "\n";

    return blob;
}

std::optional<std::vector<uint8_t>> DB::get_user_cover(const std::string& usertag) const {
    if (!connected_) {
        std::cerr << "[DB] get_user_blob: not connected to database\n";
        return std::nullopt;
    }

    std::string query =
        "SELECT profile_blob "
        "FROM users "
        "WHERE usertag = '" + usertag + "'";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results results;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_user_blob() SQL error: " << ec.message() << "\n";
        return std::nullopt;
    }
    if (results.rows().empty()) {
        std::cerr << "[DB] get_user_blob() no row for usertag = " << usertag << "\n";
        return std::nullopt;
    }

    auto row = *results.rows().begin();
    if (row.at(0).is_null()) {
        std::cerr << "[DB] get_user_blob() blob is NULL for usertag = " << usertag << "\n";
        return std::nullopt;
    }

    boost::mysql::blob_view blob_view = row.at(0).as_blob();

    std::vector<uint8_t> blob;
    blob.resize(blob_view.size());
    std::memcpy(blob.data(), blob_view.data(), blob_view.size());

    std::cout << "[DB] get_user_blob() fetched " << blob.size()
              << " bytes for usertag = " << usertag << "\n";

    return blob;
}

bool DB::is_following(const std::string& subscriber_usertag, const std::string& target_usertag) const
{
    if (!connected_) return false;

    std::string query =
    "SELECT 1 "
    "FROM subscriptions "
    "WHERE subscriber_usertag = '" + subscriber_usertag + "' "
    "  AND target_usertag = '" + target_usertag + "' "
    "LIMIT 1";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        log_error(ec, diag, "is_following");
        return false;
    }
    return !result.rows().empty();
}

std::vector<std::string> DB::search_users(const std::string& query) const {
    std::vector<std::string> usertags;
    if (!connected_) return usertags;

    std::string sql =
        "SELECT usertag FROM users "
        "WHERE username LIKE '%" + query + "%' OR usertag LIKE '%" + query + "%' "
        "LIMIT 5";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(sql, result, ec, diag);

    if (ec) {
        log_error(ec, diag, "search_users");
        return usertags;
    }

    for (const auto& row : result.rows()) {
        usertags.push_back(std::string{ row.at(0).as_string() });
    }

    return usertags;
}

std::vector<int> DB::search_albums(const std::string& query) const {
    std::vector<int> album_ids;
    if (!connected_) return album_ids;

    std::string sql =
        "SELECT album_id FROM albums "
        "WHERE title LIKE '%" + query + "%' OR description LIKE '%" + query + "%' "
        "LIMIT 5";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(sql, result, ec, diag);

    if (ec) {
        log_error(ec, diag, "search_albums");
        return album_ids;
    }

    for (const auto& row : result.rows()) {
        album_ids.push_back(static_cast<int>(row.at(0).as_int64()));
    }

    return album_ids;
}

std::vector<int> DB::search_tracks(const std::string& query) const {
    std::vector<int> track_ids;
    if (!connected_) return track_ids;

    std::string sql =
        "SELECT track_id FROM tracks "
        "WHERE title LIKE '%" + query + "%' OR description LIKE '%" + query + "%' OR track_text LIKE '%" + query + "%' "
        "LIMIT 5";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results result;
    conn_.execute(sql, result, ec, diag);

    if (ec) {
        log_error(ec, diag, "search_tracks");
        return track_ids;
    }

    for (const auto& row : result.rows()) {
        track_ids.push_back(static_cast<int>(row.at(0).as_int64()));
    }

    return track_ids;
}

std::vector<json> DB::get_followers(const std::string& usertag) const {
    std::vector<json> v;
    if (!connected_) return v;

    std::string query =
        "SELECT s.subscriber_usertag "
        "FROM subscriptions s "
        "WHERE s.target_usertag = '" + usertag + "'";

    boost::mysql::results results;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_followers error: " << ec.message() << "\n";
        return v;
    }

    for (auto const& row : results.rows()) {
        v.push_back({
            {"usertag", row.at(0).as_string()}
        });
    }
    return v;
}

std::vector<json> DB::get_following(const std::string& usertag) const {
    std::vector<json> v;
    if (!connected_) return v;

    std::string query =
        "SELECT s.target_usertag "
        "FROM subscriptions s "
        "WHERE s.subscriber_usertag = '" + usertag + "'";

    boost::mysql::results results;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(query, results, ec, diag);
    if (ec) {
        std::cerr << "[DB] get_following error: " << ec.message() << "\n";
        return v;
    }

    for (auto const& row : results.rows()) {
        v.push_back({
            {"usertag", row.at(0).as_string()}
        });
    }
    return v;
}

bool DB::toggle_follow(const std::string& usertag1, const std::string& usertag2) {
    if (!connected_) return false;

    std::string chk =
        "SELECT 1 FROM subscriptions "
        "WHERE subscriber_usertag = '" + usertag1 + "' "
        "AND target_usertag = '" + usertag2 + "' LIMIT 1";
    boost::mysql::results r;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(chk, r, ec, diag);
    if (ec) {
        std::cerr << "[DB] toggle_follow check error: " << ec.message() << "\n";
        return false;
    }
    bool exists = !r.rows().empty();

    std::string q;
    if (exists) {
        q = "DELETE FROM subscriptions WHERE subscriber_usertag = '" + usertag1 +
            "' AND target_usertag = '" + usertag2 + "'";
    } else {
        q = "INSERT INTO subscriptions (subscriber_usertag, target_usertag) VALUES ('" +
            usertag1 + "', '" + usertag2 + "')";
    }
    conn_.execute(q, r, ec, diag);
    if (ec) {
        std::cerr << "[DB] toggle_follow exec error: " << ec.message() << "\n";
        return false;
    }
    return !exists;
}

bool DB::toggle_like_track(const std::string& usertag, int track_id) {
    if (!connected_) return false;

    std::string chk =
        "SELECT 1 FROM track_likes "
        "WHERE usertag = '" + usertag + "' AND track_id = " + std::to_string(track_id) + " LIMIT 1";
    boost::mysql::results r;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(chk, r, ec, diag);
    if (ec) {
        std::cerr << "[DB] toggle_like_track check error: " << ec.message() << "\n";
        return false;
    }
    bool exists = !r.rows().empty();

    std::string q;
    if (exists) {
        q = "DELETE FROM track_likes WHERE usertag = '" + usertag +
            "' AND track_id = " + std::to_string(track_id);
    } else {
        q = "INSERT INTO track_likes (usertag, track_id) VALUES ('" + usertag +
            "', " + std::to_string(track_id) + ")";
    }
    conn_.execute(q, r, ec, diag);
    if (ec) {
        std::cerr << "[DB] toggle_like_track exec error: " << ec.message() << "\n";
        return false;
    }
    return !exists;
}

bool DB::toggle_like_album(const std::string& usertag, int album_id) {
    if (!connected_) return false;

    std::string chk =
        "SELECT 1 FROM album_likes "
        "WHERE usertag = '" + usertag + "' AND album_id = " + std::to_string(album_id) + " LIMIT 1";
    boost::mysql::results r;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(chk, r, ec, diag);
    if (ec) {
        std::cerr << "[DB] toggle_like_album check error: " << ec.message() << "\n";
        return false;
    }
    bool exists = !r.rows().empty();

    std::string q;
    if (exists) {
        q = "DELETE FROM album_likes WHERE usertag = '" + usertag +
            "' AND album_id = " + std::to_string(album_id);
    } else {
        q = "INSERT INTO album_likes (usertag, album_id) VALUES ('" + usertag +
            "', " + std::to_string(album_id) + ")";
    }
    conn_.execute(q, r, ec, diag);
    if (ec) {
        std::cerr << "[DB] toggle_like_album exec error: " << ec.message() << "\n";
        return false;
    }
    return !exists;
}

bool DB::is_album_liked(const std::string& usertag, int album_id) const {
    if (!connected_) return false;

    std::string chk =
        "SELECT 1 FROM album_likes "
        "WHERE usertag = '" + usertag + "' AND album_id = " + std::to_string(album_id) + " LIMIT 1";
    boost::mysql::results r;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(chk, r, ec, diag);
    if (ec) {
        std::cerr << "[DB] is_album_liked error: " << ec.message() << "\n";
        return false;
    }
    return !r.rows().empty();
}

bool DB::set_mytracks(const std::string& usertag, int flag) {
    if (!connected_) return false;
    std::string query = "UPDATE users SET mytracks = " + std::to_string(flag ? 1 : 0) + " WHERE usertag = '" + usertag + "'";
    boost::mysql::results result;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "set_mytracks execute error: " << ec.message() << "\n";
        return false;
    }
    return true;
}

bool DB::set_myloadedtracks(const std::string& usertag, int flag) {
    if (!connected_) return false;
    std::string query = "UPDATE users SET myloadedtracks = " + std::to_string(flag ? 1 : 0) + " WHERE usertag = '" + usertag + "'";
    boost::mysql::results result;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "set_myloadedtracks execute error: " << ec.message() << "\n";
        return false;
    }
    return true;
}

bool DB::set_myalbums(const std::string& usertag, int flag) {
    if (!connected_) return false;
    std::string query = "UPDATE users SET myalbums = " + std::to_string(flag ? 1 : 0) + " WHERE usertag = '" + usertag + "'";
    boost::mysql::results result;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "set_myalbums execute error: " << ec.message() << "\n";
        return false;
    }
    return true;
}

bool DB::set_myloadedalbums(const std::string& usertag, int flag) {
    if (!connected_) return false;
    std::string query = "UPDATE users SET myloadedalbums = " + std::to_string(flag ? 1 : 0) + " WHERE usertag = '" + usertag + "'";
    boost::mysql::results result;
    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    conn_.execute(query, result, ec, diag);
    if (ec) {
        std::cerr << "set_myloadedalbums execute error: " << ec.message() << "\n";
        return false;
    }
    return true;
}

bool DB::set_default_profile_cover(const std::string& usertag) const
{
    if (!connected_) {
        std::cerr << "[DB] set_default_profile_cover: not connected to database\n";
        return false;
    }

    const std::string filename = "../../../../common/base_profile.jpg";
    if (!std::filesystem::exists(filename)) {
        std::cerr << "[DB] File not found at: " << filename << "\n";
    } else {
        std::cerr << "[DB] File exists: " << filename << "\n";
    }

    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "[DB] set_default_profile_cover: cannot open file '"
                  << filename << "'\n";
        return false;
    }

    std::vector<uint8_t> blob{
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    };
    std::cerr << "[DB] set_default_profile_cover: read "
              << blob.size() << " bytes from '" << filename << "'\n";


    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    auto stmt = conn_.prepare_statement(
        "UPDATE users SET profile_blob = ? WHERE usertag = ?",
        ec, diag
    );
    if (ec) {
        std::cerr << "[DB] set_default_profile_cover: prepare_statement failed: "
                  << ec.message() << "\n";
        return false;
    }

    boost::mysql::blob_view bv{ blob.data(), blob.size() };
    auto bound = stmt.bind(bv, usertag);
    std::cerr << "[DB] set_default_profile_cover: bound parameters, now executing\n";

    boost::mysql::results result;
    conn_.execute(bound, result, ec, diag);
    if (ec) {
        std::cerr << "[DB] set_default_profile_cover: execute failed: "
                  << ec.message() << "\n";
        return false;
    }

    if (result.affected_rows() == 0) {
        std::cerr << "[DB] set_default_profile_cover: no rows updated for usertag='"
                  << usertag << "'\n";
        return false;
    }

    std::cerr << "[DB] set_default_profile_cover: SUCCESS for usertag='"
              << usertag << "'\n";
    return true;
}

int DB::save_profile_cover(const std::string& usertag, const std::vector<uint8_t>& profile_blob) const
{
    if (!connected_) return -1;

    std::string hexCover = blob_to_hex(profile_blob);

    std::string q =
        "UPDATE users SET profile_blob = x'" + hexCover + "' WHERE usertag = '" + usertag + "'";

    boost::mysql::error_code ec;
    boost::mysql::diagnostics diag;
    boost::mysql::results r;
    conn_.execute(q, r, ec, diag);
    if (ec) {
        log_error(ec, diag, "save_profile_cover");
        return -1;
    }

    return 0;
}
