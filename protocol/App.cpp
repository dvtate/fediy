#include "seastar/core/thread.hh"

#include "App.hpp"


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

    m_app.run(argc, argv, [&] () {
        return seastar::async([&]() {

        });
    });

}