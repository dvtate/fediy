#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

#include "Module.hpp"
#include "ModuleInfo.hpp"
#include "Message.hpp"

class ModuleManager {
public:
    std::list<Module> m_modules;
    std::map<std::string, Module> m_name_map;
    
    ModuleManager();
    ~ModuleManager();

    void remove_module(const std::string& name);
    void add_module(const std::string& path);
    void update_module(const std::string& path);

    void start_modules();

protected:
    void load_modules();

};