#include "search-user-handler.h"

std::vector<std::string> SearchUserHandler::handle_search_user(const std::string& query) const {
    return db_.search_users(query);
}

http::response<http::string_body> SearchUserHandler::operator()(int http_version, const json& body) const {
    http::response<http::string_body> res{http::status::ok, http_version};
    res.set(http::field::content_type, "application/json");

    if (!body.contains("query") || !body["query"].is_string()) {
        res.result(http::status::bad_request);
        res.body() = R"({"status":"error","message":"Missing or invalid 'query'"})";
        res.prepare_payload();
        return res;
    }

    std::string query = body["query"].get<std::string>();

    try {
        std::vector<std::string> usertags = handle_search_user(query);

        json resp_body;
        resp_body["status"] = "ok";
        resp_body["usertags"] = usertags;

        res.body() = resp_body.dump();
    } catch (const std::exception& e) {
        res.result(http::status::internal_server_error);
        res.body() = std::string(R"({"status":"error","message":"Exception during search_user: )") + e.what() + "\"}";
    }

    res.prepare_payload();
    return res;
}