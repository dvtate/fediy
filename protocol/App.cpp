#include "seastar/core/thread.hh"

#include "App.hpp"

#include "routes.hpp"

void App::run(int argc, char** argv) {
    // Note: order is important here
    if (m_config.m_error) {
        LOG_ERR("Failed to parse config file.");
        return;
    }

    // Connect to datbase
    m_db = std::make_unique<DB>(m_config.m_data_dir + "/db.db3");

    // Start modules
    if (!m_mods.start_all()) {
        LOG_ERR("Failed to start modules.");
        return;
    }

    m_pages = std::make_unique<Pages>();

    m_app.run(argc, argv, [&] () {
        return seastar::async([&]() {
            auto server = std::make_unique<seastar::httpd::http_server_control>();
            server->set_routes(add_server_routes).get();
            server->listen((uint16_t) 8082).get();

        });
    });

}