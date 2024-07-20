#include <filesystem>

#include "App.hpp"

#include "Mods.hpp"

void Mods::find_modules() {
    auto apps_dir = g_app->m_config.m_data_dir + "/apps";
    std::string fail_reason;

    std::cout <<apps_dir <<std::endl;

    // TODO parallel
    for (auto& p : std::filesystem::directory_iterator(apps_dir))
        if (p.is_directory()) {
            auto&& id = p.path().filename().string();
            m_mods.emplace_back(new Mod(id));
        }

    populate_caches();

    DEBUG_LOG("Found " + std::to_string(m_mods.size()) + " apps");
}

bool Mods::start_all() {
    bool ret = true;

    // TODO parallel
    for (auto* mod: m_mods) {
        if (!mod->m_loaded)
            continue;
        DEBUG_LOG("Starting module: " + mod->m_id + "...");
        if (!mod->start()) {
            LOG("Failed to start module " + mod->m_id);
            ret = false;
        } else {
            LOG("Successfully started module " + mod->m_id);
        }
    }
    return ret;
}


bool Mods::stop_all() {
    bool ret = true;
    for (auto* mod : m_mods)
        if (mod->status() == Mod::Status::RUNNING)
            if (!mod->stop())
                ret = false;
    return ret;
}