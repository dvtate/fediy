#pragma once

#include <map>

#include "Mod.hpp"

class ModMgr {
    std::map<std::string, Mod*> m_mods;

    void find_modules();

public:
    ModMgr() {
        find_modules();
    }

    ~ModMgr() {
        for (auto& [id, mod]: m_mods)
            delete mod;
    }

    bool start_all();

    Mod* get_mod(const std::string& id) {
        auto ret = m_mods.find(id);
        return (ret != m_mods.end())
            ? ret->second
            : nullptr;
    }
};
