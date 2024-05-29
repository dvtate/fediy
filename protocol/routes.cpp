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



std::string user_token_set_cookie_string(std::string&& token) {
    // 2 weeks = 60 * 60 * 24 * 14 = 1209600 seconds
    return "sessionToken=" + token + "; Max-Age=1209600; Secure; HttpOnly";
}

class LoginPageHandlerPost : public httpd::handler_base {
    future<std::unique_ptr<http::reply>>
    handle(
            const sstring& path,
            std::unique_ptr<http::request> req, std::unique_ptr<http::reply> rep
    ) override {
        req->content
        Auth::auth_local_user()

        rep->set_status(seastar::http::reply::status_type::moved_temporarily);
        rep->add_header("Location", "/portal");

        return make_ready_future<std::unique_ptr<http::reply>>(std::move(rep));
    }

};

class ModHandler : public httpd::handler_base {

};

void add_routes(seastar::httpd::routes& routes) {
    routes.add_default_handler(new Error404Handler());
}