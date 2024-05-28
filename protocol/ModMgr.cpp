#include <filesystem>

#include "App.hpp"

#include "ModMgr.hpp"

void ModMgr::find_modules() {
    auto apps_dir = g_app->m_config.m_data_dir + "/apps";
    for (auto& p : std::filesystem::directory_iterator(apps_dir))
        if (p.is_directory()) {
            auto id = p.path().filename().string();
            m_mods.emplace(id, new Mod(id));
        }
    DEBUG_LOG("Found " + std::to_string(m_mods.size()) + " apps");
}


bool ModMgr::start_all() {
    bool ret = true;
    for (auto& [id, mod]: m_mods) {
        DEBUG_LOG("Starting module: " + id + "...");
        if (!mod->load_conf()) {
            LOG("Failed to load conf for module " + id);
            ret = false;
            continue;
        }

        if (!mod->start_module()) {
            LOG("Failed to start module " + id);
            ret = false;
        } else {
            LOG("Successfully started module " + id);
        }
    }
    return ret;
}