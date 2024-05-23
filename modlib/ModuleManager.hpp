#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

#include "Module.hpp"
#include "ModuleInfo.hpp"
#include "Msg.hpp"

class ModuleManager {
protected:

    std::list<std::unique_ptr<Module>> m_modules;
    std::map<std::string, Module*> m_name_map; // Pointers to Modules in m_modules

public:

    ModuleManager();
    ~ModuleManager();

    // TODO for now this will require restarting system..
    //      need cut it off when updating
    // void remove_module(const std::string& name);
    // void add_module(const std::string& path);
    // void update_module(const std::string& path);

    void start_modules();

    ModuleInfo::Compatibility get_info(const std::string& module, uint64_t version, ModuleInfo& mod_info, ModuleInfo* target) {
        auto modit = m_name_map.find(module);
        if (modit == m_name_map.end())
            return ModuleInfo::Compatibility::NAME_MISMATCH;
        mod_info.set_version(version);
        if (modit->second->m_info.m_version_major != mod_info.m_version_major)
            return ModuleInfo::Compatibility::VERSION_MISMATCH;
        mod_info.m_name = modit->second->m_info.m_name;
        return ModuleInfo::Compatibility::COMPATIBLE;
    }

    Msg make_msg(ModuleInfo minfo, , bool reply_expected = true) {

    }

protected:


    void load_modules();

};