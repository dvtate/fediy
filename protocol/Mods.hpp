#pragma once

#include <map>

#include "Mod.hpp"

// TODO refactor so that it handles ids and path lookups better

/**
 * Interface and cache for managing the installed apps
 */
class Mods {
protected:
    std::map<std::string, std::unique_ptr<Mod>> m_mods;
    std::mutex m_mtx;
    std::vector<Mod*> m_mods_list_cache;
    volatile bool m_mods_list_cache_valid{false};

public:

    void find_modules();

    bool start_all();

    const Mod* get_mod_by_id(const std::string& id) {
        // TODO this should be O(1)
        for (const Mod* m: this->get_mods())
            if (m->m_id == id)
                return m;
        return nullptr;
    }

    std::unique_ptr<Mod>& get_mod(const std::string& path) {
        auto ret = m_mods.find(path);
        if (ret != m_mods.end())
            return ret->second;

        static std::unique_ptr<Mod> not_found = nullptr;
        return not_found;
    }

    const std::vector<Mod*>& get_mods() {
        // TODO ????

        // Using shared, cached result
        if (m_mods_list_cache_valid)
            return m_mods_list_cache;

        // Check if another thread already finished it
        m_mtx.lock();
        if (m_mods_list_cache_valid)
            return m_mods_list_cache;

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
        m->m_mtx.lock();
        if (m == nullptr)
            return false;
        m_mods.erase(id);
        return m->stop();
    }

    bool update_id(const std::string& old_path, const std::string& new_path) {
        std::lock_guard lock(m_mtx);
        if (m_mods.contains(new_path))
            return false;
        m_mods[new_path] = std::move(m_mods[old_path]);
        m_mods.erase(old_path);
//        m_mods[new_path]->stop();
        m_mods[new_path]->set_path(new_path);
//        m_mods[new_id]->start();
        return true;
    }
};
