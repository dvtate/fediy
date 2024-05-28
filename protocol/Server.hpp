#pragma once

#include "seastar/core/future.hh"
#include "seastar/http/httpd.hh"
#include "seastar/http/function_handlers.hh"
#include "seastar/http/request.hh"

class Server {




    seastar::future<std::variant<std::shared_ptr<LocalUser>, std::shared_ptr<Peer>>> get_authenticated_user(const );

    void set_routes(seastar::httpd::routes& routes);

};

