#pragma once

#include <map>

#include "Mod.hpp"

class ModMgr {
    std::map<std::string, std::unique_ptr<Mod>> m_mods;

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

    std::vector<Mod*> get_mods() {
        std::vector<Mod*> ret;
        ret.reserve(m_mods.size());
        for (auto& [id, mod]: m_mods)
            ret.emplace_back(&*mod);
        return ret;
    }

    bool remove_mod(const std::string& id) {
        auto m = std::move(get_mod(id));
        if (m == nullptr)
            return false;
        m_mods.erase(id);
        return m->stop();
    }
};
