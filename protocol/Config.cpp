#include "../third_party/inih/ini.h"

#include "Config.hpp"

bool Config::parse(const std::string& path) {
    LOG("Loading Config file: " << path);
    return ini_parse(
        path.c_str(),
        [](void* cfg, auto section, auto key, auto value){ 
            return ((Config*) cfg)->set_key(section, key, value); 
        },
        this
    ) == 0;
}