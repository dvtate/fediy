#include "seastar/core/thread.hh"

#include "App.hpp"


void App::run(int argc, char** argv) {
    // Note: order is important here
    if (!m_config.parse(FEDIY_CONFIG_FILE_PATH)) {
        LOG_ERR("Failed to parse config file.");
        return;
    }
    if (!m_db.connect()) {
        LOG_ERR("Failed to connect to database.");
        return;
    }
    if (!m_auth.init()) {
        LOG_ERR("Failed to initialize Authentication component.");
        return;
    }
    if (!m_mods.start_all()) {
        LOG_ERR("Failed to start modules.");
        return;
    }

    m_app.run(argc, argv, [&] () {
        return seastar::async([&]() {

        });
    });

}