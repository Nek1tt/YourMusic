#include "create-handler.h"
#include <boost/beast/core/buffer_traits.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <openssl/evp.h>

std::vector<uint8_t> CreateHandler::decode_base64(const std::string& b64) {
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

http::response<http::string_body> CreateHandler::operator()(int http_version, const json& body_json) {
    http::response<http::string_body> res{ http::status::ok, http_version };
    res.set(http::field::content_type, "application/json");

    if (!body_json.contains("entity") || !body_json["entity"].is_string()) {
        res.result(http::status::bad_request);
        res.body() = R"({"status":"error","message":"Missing or invalid 'entity'"})";
        res.prepare_payload();
        return res;
    }
    std::string entity = body_json["entity"].get<std::string>();

    try {
        if (entity == "album") {
            // ------------------ CREATE ALBUM ------------------
            // Обязательные поля:
            //   album_title     (string)
            //   usertags        (array of strings)
            //   cover_blob      (base64 string)
            //   tracks          (array of объектов)
            if (!body_json.contains("album_title")   || !body_json["album_title"].is_string()  ||
                !body_json.contains("usertags")      || !body_json["usertags"].is_array()     ||
                !body_json.contains("cover_blob")    || !body_json["cover_blob"].is_string()   ||
                !body_json.contains("tracks")        || !body_json["tracks"].is_array())
            {
                res.result(http::status::bad_request);
                res.body() = R"({"status":"error","message":"Missing required album fields"})";
                res.prepare_payload();
                return res;
            }

            std::string album_title  = body_json["album_title"].get<std::string>();
            std::string album_description = body_json.value("description", std::string{});

            // 1) Создаём пустой альбом, получаем album_id
            int album_id = db_.create_empty_album(album_title, album_description);
            if (album_id < 0) {
                throw std::runtime_error("Failed to create album record");
            }

            // 2) Сохраняем связь album_authors (используем массив usertags)
            for (auto& ut : body_json["usertags"]) {
                if (!ut.is_string()) {
                    throw std::runtime_error("Invalid usertag in usertags[]");
                }
                db_.add_album_author(album_id, ut.get<std::string>());
            }

            // 3) Декодируем cover_blob и сохраняем в BLOB
            std::string cover_b64 = body_json["cover_blob"].get<std::string>();
            std::vector<uint8_t> cover_blob = decode_base64(cover_b64);
            db_.store_album_cover(album_id, cover_blob);

            // 4) Обрабатываем каждый трек в массиве "tracks"
            std::vector<int> created_track_ids;
            for (auto& tr : body_json["tracks"]) {
                // Ожидаем, что каждый элемент трек-объекта содержит:
                //   title             (string)
                //   duration_seconds  (integer)
                //   file_blob         (base64 string)
                //   usertags          (array of strings)
                if (!tr.contains("title")            || !tr["title"].is_string()            ||
                    !tr.contains("duration_seconds") || !tr["duration_seconds"].is_number_integer() ||
                    !tr.contains("file_blob")        || !tr["file_blob"].is_string()        ||
                    !tr.contains("usertags")         || !tr["usertags"].is_array())
                {
                    throw std::runtime_error("Missing required track fields");
                }

                std::string track_title    = tr["title"].get<std::string>();
                int duration_seconds       = tr["duration_seconds"].get<int>();
                std::string file_b64       = tr["file_blob"].get<std::string>();
                std::vector<uint8_t> file_blob = decode_base64(file_b64);

                // Вставляем трек (используем album_id из пункта 1)
                int track_id = db_.create_track(album_id, track_title, duration_seconds, file_blob, cover_blob);
                if (track_id < 0) {
                    throw std::runtime_error("Failed to create track record");
                }
                created_track_ids.push_back(track_id);

                // Сохраняем связь track_authors (используем массив "usertags" из объекта трека)
                for (auto& ut : tr["usertags"]) {
                    if (!ut.is_string()) {
                        throw std::runtime_error("Invalid usertag in track's usertags[]");
                    }
                    db_.add_track_author(track_id, ut.get<std::string>());
                }
            }

            // 5) Формируем успешный ответ
            json resp;
            resp["status"]    = "ok";
            resp["album_id"]  = album_id;
            resp["track_ids"] = created_track_ids;

            res.body() = resp.dump();
            res.prepare_payload();
            return res;
        }
        else if (entity == "track") {
            // ------------------ CREATE SINGLE TRACK ------------------
            // Обязательные поля:
            //   title             (string)
            //   duration_seconds  (integer)
            //   file_blob         (base64 string)
            //   cover_blob        (base64 string)
            //   usertags          (array of strings)
            if (!body_json.contains("title")            || !body_json["title"].is_string()            ||
                !body_json.contains("duration_seconds") || !body_json["duration_seconds"].is_number_integer() ||
                !body_json.contains("file_blob")        || !body_json["file_blob"].is_string()        ||
                !body_json.contains("usertags")         || !body_json["usertags"].is_array()          ||
                !body_json.contains("cover_blob")       || !body_json["cover_blob"].is_string())
            {
                res.result(http::status::bad_request);
                res.body() = R"({"status":"error","message":"Missing required track fields"})";
                res.prepare_payload();
                return res;
            }

            std::string track_title = body_json["title"].get<std::string>();
            int duration_seconds = body_json["duration_seconds"].get<int>();
            std::string file_b64 = body_json["file_blob"].get<std::string>();
            std::string cover_b64 = body_json["cover_blob"].get<std::string>();
            std::vector<uint8_t> file_blob = decode_base64(file_b64);
            std::vector<uint8_t> cover_blob = decode_base64(cover_b64);
            // Опциональные поля
            std::string description = body_json.value("description", std::string{});
            std::string track_text  = body_json.value("track_text",  std::string{});
            // 1) Создаём запись с album_id = NULL (0 => no album)
            int track_id = db_.create_track(0, track_title, duration_seconds, file_blob, cover_blob, description, track_text);
            if (track_id < 0) {
                throw std::runtime_error("Failed to create track record");
            }

            // 2) Сохраняем связь track_authors (используем массив usertags)
            for (auto& ut : body_json["usertags"]) {
                if (!ut.is_string()) {
                    throw std::runtime_error("Invalid usertag in usertags[]");
                }
                db_.add_track_author(track_id, ut.get<std::string>());
            }

            // 3) Формируем успешный ответ
            json resp;
            resp["status"]   = "ok";
            resp["track_id"] = track_id;

            res.body() = resp.dump();
            res.prepare_payload();
            return res;
        }
        else {
            res.result(http::status::bad_request);
            res.body()   = R"({"status":"error","message":"Invalid 'entity' value"})";
            res.prepare_payload();
            return res; 
        }
    }
    catch (const std::exception& e) {
        res.result(http::status::internal_server_error);
        json err;
        err["status"]  = "error";
        err["message"] = e.what();
        res.body() = err.dump();
        res.prepare_payload();
        return res;
    }
}
