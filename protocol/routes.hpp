#pragma once

#include "seastar/http/httpd.hh"


void add_server_routes(seastar::httpd::routes& routes);