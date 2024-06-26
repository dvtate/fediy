#include "App.hpp"

#include "HTTPRoutes/PortalRoutes.hpp"
#include "HTTPRoutes/ModuleRoutes.hpp"
#include "HTTPRoutes/PeerRoutes.hpp"

bool App::start() {
    // Note: order is important

    if (m_config.m_error) {
        LOG_ERR("Failed to parse config file.");
        return false;
    }

    // Connect to datbase
    m_db = std::make_unique<DB>();

    // Start modules
    m_mods.find_modules();
    if (!m_mods.start_all()) {
        LOG_ERR("Failed to start modules.");
        return false;
    }

    m_pages = std::make_unique<Pages>();

    PortalRoutes::initPathRouting();

    return true;
}
