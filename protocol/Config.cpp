#include "../third_party/inih/ini.h"

#include "Config.hpp"

bool Config::parse(const std::string& path) {
    LOG("Loading Config file: " << path);
    return m_error = ini_parse(
        path.c_str(),
        [](void* cfg, auto section, auto key, auto value){
            return (int)((Config*) cfg)->set_key(section, key, value);
        },
        this
    );
}

bool AppConfig::set_key(const char* section, const char* key, const char* value) {
    // No section
    if (section[0] != '\0')
        return false;

    if (strcmp(key, "data_dir") == 0) {
        // Where app files are stored
        std::error_code ec;
        if (!std::filesystem::is_directory(value, ec)) {
            LOG_ERR("Config file: data_dir invalid path: " << value << std::endl);
        }
        if (ec.value() != 0) {
            LOG_ERR("filesystem::is_directory failed: " << ec.message() << std::endl);
        }
        m_data_dir = value;
    } else if (strcmp(key, "hostname") == 0) {
        // Validate
        const std::regex domain_name_pattern{ // supports subdomains and port numbers
            R"(^(?!-)[A-Za-z0-9-]+([\-\.]{1}[a-z0-9]+)*\.[A-Za-z]{2,6}(?:\:[0-9]{1,5})?$)"};
        if (!std::regex_match(value, domain_name_pattern)) {
            LOG_ERR("Config file: hostname '" << value << "' is invalid (valid example: example.com)");
        }

        // Apply
        m_hostname = (char*) malloc(strlen(value) + 1);
        strcpy(m_hostname, value);
    } else if (strcmp(key, "ssl") == 0) {
        int b = parse_bool(value);
        if (b == -1) {
            LOG_ERR("Config file: ssl should be set to true if the server can be accessed via https. Otherwise set it to false.");
        }
        m_ssl = b;
    } else if (strcmp(key, "salt") == 0) {
        m_salt = value;
    } else if (strcmp(key, "port") == 0) {
        int port = strtol(value, nullptr, 10);
        if (port == 0 || port < 0 || port >= 65536) {
            LOG_ERR("Config file: port should be a valid port number, not " <<value);
        } else {
            m_port = port;
        }
    } else {
        LOG_ERR("Config file: invalid key: " <<key);
        return false; // invalid key
    }
    return true;
}