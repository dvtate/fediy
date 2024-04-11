#include <cstring>

#include "../third_party/inih/ini.h"

#include "Config.hpp"


ServerConfig g_server_config;

int Config::ini_parse_handler(void* cfg, const char* section, const char* key, const char* value) {
    const char* cfgsec = ((Config*) cfg)->section();
    if (strcmp(section, cfgsec) == 0)
        return (int) ((Config*) cfg)->set_key(key, value);
    return 0;
}

bool Config::parse(const std::string& path) {
    ini_parse(path.c_str(), Config::ini_parse_handler, this);

}