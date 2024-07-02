#pragma once

#include <map>

#include "Mod.hpp"

class ModMgr {
protected:
    std::map<std::string, std::unique_ptr<Mod>> m_mods;
    std::mutex m_mtx;
    std::vector<Mod*> m_mods_list_cache;
    bool m_mods_list_cache_valid{false};

public:

    void find_modules();

    bool start_all();

    std::unique_ptr<Mod>& get_mod(const std::string& id) {
        auto ret = m_mods.find(id);
        if (ret != m_mods.end())
            return ret->second;

        static std::unique_ptr<Mod> not_found = nullptr;
        return not_found;
    }

    const std::vector<Mod*>& get_mods() {
        // Using shared, cached result
        if (m_mods_list_cache_valid)
            return m_mods_list_cache;

        // Wait for earlier thread to finish
        if (!m_mtx.try_lock()) {
            m_mtx.lock();
            m_mtx.unlock();
            return m_mods_list_cache;
        }
        m_mods_list_cache.clear();
        m_mods_list_cache.reserve(m_mods.size());
        for (auto& [id, mod]: m_mods)
            m_mods_list_cache.emplace_back(&*mod);
        m_mods_list_cache_valid = true;
        m_mtx.unlock();
        return m_mods_list_cache;
    }

    bool remove_mod(const std::string& id) {
        std::lock_guard lock(m_mtx);
        m_mods_list_cache_valid = false;
        auto m = std::move(get_mod(id));
        if (m == nullptr)
            return false;
        m_mods.erase(id);
        return m->stop();
    }
};
