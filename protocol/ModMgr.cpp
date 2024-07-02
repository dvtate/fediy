#include <filesystem>

#include "App.hpp"

#include "ModMgr.hpp"

void ModMgr::find_modules() {
    auto apps_dir = g_app->m_config.m_data_dir + "/apps";
    std::string fail_reason;

    // TODO parallel
    for (auto& p : std::filesystem::directory_iterator(apps_dir))
        if (p.is_directory()) {
            auto id = p.path().filename().string();
            auto m = Mod::load(id, fail_reason);
            if (m != nullptr) {
                m_mods.emplace(id, std::move(m));
            }
        }
    DEBUG_LOG("Found " + std::to_string(m_mods.size()) + " apps");
}

bool ModMgr::start_all() {
    bool ret = true;
    for (auto& [id, mod]: m_mods) {
        if (!mod->m_loaded)
            continue;
        DEBUG_LOG("Starting module: " + id + "...");
        if (!mod->start()) {
            LOG("Failed to start module " + id);
            ret = false;
        } else {
            LOG("Successfully started module " + id);
        }
    }
    return ret;
}