#include "../third_party/inih/ini.h"

#include "Config.hpp"


ServerConfig g_server_config;

bool Config::parse(const std::string& path) {
    return ini_parse(
        path.c_str(),
        [](void* cfg, auto section, auto key, auto value){ 
            return ((Config*) cfg)->set_key(section, key, value); 
        },
        this
    ) == 0;
}