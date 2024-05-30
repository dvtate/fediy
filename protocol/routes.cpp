//
// Created by tate on 5/29/24.
//

#include "seastar/core/future.hh"
#include "seastar/http/httpd.hh"
#include "seastar/http/function_handlers.hh"
#include "seastar/http/request.hh"

#include "App.hpp"

#include "routes.hpp"

using namespace seastar;

class Error404Handler : public httpd::handler_base {
public:
    future<std::unique_ptr<http::reply>>
    handle(
            const sstring& path,
            std::unique_ptr<http::request> req, std::unique_ptr<http::reply> rep
    ) override {
        rep->set_status(http::reply::status_type::not_found, "404 - not found");
        rep->_content = "404 not";
        rep->done("html");
        return make_ready_future<std::unique_ptr<http::reply>>(std::move(rep));
    }
};

class AuthenticatedHandler : public httpd::handler_base {
protected:
    std::string m_user;
    bool m_localuser;
public:
    future<std::unique_ptr<http::reply>>
    handle(
            const sstring& path,
            std::unique_ptr<http::request> req, std::unique_ptr<http::reply> rep
    ) final {
        authenticated_handler(path, req, rep);
        return make_ready_future<std::unique_ptr<http::reply>>(std::move(rep));
    }

    virtual void
    authenticated_handler(
            const sstring& path,
            std::unique_ptr<http::request>& req, std::unique_ptr<http::reply>& rep
    ) = 0;
};

// parse application/x-www-form-urlencoded
bool parse_form_url_encoded(seastar::sstring& s, std::deque<std::pair<seastar::sstring, seastar::sstring>>& ret) {
    size_t i = 0;
    do {
        size_t start = i;
        std::pair<seastar::sstring, seastar::sstring> kv;
        while (i < s.size() && s[i] != '=')
            i++;
        if (i == s.size() || s[i] != '=')
            return false;
        if (!seastar::http::internal::url_decode(s.substr(start, i-start), kv.first))
            return false;
        start = ++i;
        while (i < s.size() && s[i] != '&')
            i++;
        if (i == s.size() || s[i] != '&')
            return false;
        if (!seastar::http::internal::url_decode(s.substr(start, i-start), kv.second))
            return false;
        ret.emplace_back(std::move(kv));
        i++;
    } while (i < s.size());
    return true;
}

class LoginPageHandlerPost : public httpd::handler_base {
    future<std::unique_ptr<http::reply>> handle(
        const sstring& path,
        std::unique_ptr<http::request> req, std::unique_ptr<http::reply> rep
    ) override {
        // Parse form body
        std::string username;
        std::string password;
        std::deque<std::pair<seastar::sstring, seastar::sstring>> form;
        if (!parse_form_url_encoded(req->content, form)) {
            rep->set_status(seastar::http::reply::status_type::bad_request, "Invalid body");
            LOG_ERR("invalid body: " <<req->content);
            return make_ready_future<std::unique_ptr<http::reply>>(std::move(rep));
        }
        for (const auto& [k, v] : form) {
            if (k == "username")
                username = v;
            else if (k == "password")
                password = v;
        }

        // Get user from database
        auto token = LocalUser::login(username, password);
        if (token.empty()) {
            rep->set_status(seastar::http::reply::status_type::unauthorized, "Bad credentials");
            return make_ready_future<std::unique_ptr<http::reply>>(std::move(rep));
        }

        // Add auth token to cookies
        // 2 weeks = 60 * 60 * 24 * 14 = 1209600 seconds
        rep->add_header("Set-Cookie", "sessionToken=" + token + "; Max-Age=1209600; Secure; HttpOnly");

        // Redirect user to the portal
        rep->set_status(seastar::http::reply::status_type::moved_temporarily);
        rep->add_header("Location", "/portal");

        return make_ready_future<std::unique_ptr<http::reply>>(std::move(rep));
    }
};

class ModHandler : public httpd::handler_base {

};

void add_routes(seastar::httpd::routes& routes) {
    routes.add_default_handler(new Error404Handler());
    routes.add(httpd::POST, httpd::url("/portal/login"), new LoginPageHandlerPost());
}
