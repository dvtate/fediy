
#include "Peer.hpp"
#include "LocalUser.hpp"

#include "Server.hpp"

using namespace seastar;


class Error404Handler : public httpd::handler_base {
public:
    virtual future<std::unique_ptr<http::reply>>
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

void set_routes(httpd::routes& routes) {
/*
    Endpoints:

    /portal/*  -- management portal frontend
        - GET|POST /login
            - set cookie
        - GET|POST /register
            - set cookie
        - GET|POST /home
        - GET|POST /user/settings
        - GET|POST /admin/settings

    /protocol  -- establish p2p channel
        - GET /pubkey
        - POST /connect

    /[app_id]/* -- route to relevant app
*/
    // 404 page
    routes.add_default_handler(new Error404Handler());


//    routes.add(httpd::operation_type::GET, "/portal/login", new httpd::function_handler([g_app](const httpd::request& )));
}