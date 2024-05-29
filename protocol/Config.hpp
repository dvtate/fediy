#pragma once

#include <string>
#include <fstream>
#include <cctype>
#include <cinttypes>
#include <filesystem>
#include <cstring>
#include <regex>

#include "globals.hpp"

/**
 * INI Config file abstract base class
 */
class Config {
public:
    Config() = default;
    virtual ~Config() = default;

    bool parse(const std::string& path);

protected:
    virtual int set_key(const char* section, const char* key, const char* value) = 0;

    int parse_bool(const char* str) {
        if (strcasecmp(str, "true") == 0 || strcasecmp(str, "y") == 0 || strcmp(str, "1") == 0)
            return 1;
        if (strcasecmp(str, "false") == 0 || strcasecmp(str, "n") == 0 || strcmp(str, "0") == 0)
            return 0;
        return -1;
    }
};


class AppConfig : public Config {
public:
    AppConfig() = default;
    virtual ~AppConfig() = default;

    /// Where files are stored
    std::string m_data_dir{"/opt/fediy/data"};

    /// Domain where we're hosting the the service
    std::string m_hostname;

    /// Should clients+peers use SSL?
    bool m_ssl{true};

protected:
    virtual int set_key(const char* section, const char* key, const char* value) override {
        // No section
        if (section[0] != '\0')
            return false;

        // Where files are stored
        if (strcmp(key, "data_dir") == 0) {
            std::error_code ec;
            if (!std::filesystem::is_directory(value, ec)) {
                LOG_ERR("Config file: data_dir invalid path: " << value << std::endl);
            }
            if (ec.value() != 0) {
                LOG_ERR("filesystem::is_directory failed: " << ec.message() << std::endl);
            }
            m_data_dir = value;
            return 0;
        } else if (strcmp(key, "hostname") == 0) {
            m_hostname = value;
            const std::regex domain_name_pattern{"^(?!-)[A-Za-z0-9-]+([\\-\\.]{1}[a-z0-9]+)*\\.[A-Za-z]{2,6}$"};

            if (!std::regex_match(value, domain_name_pattern)) {
                LOG_ERR("Config file: hostname '" << value << "' is invalid (valid example: example.com)");
            }
            return 0;
        } else if (strcmp(key, "ssl") == 0) {
            int b = parse_bool(value);
            if (b == -1) {
                LOG_ERR("Config file: ssl should be set to true if the server can be accessed via https. Otherwise set it to false.");
            }
            m_ssl = b;
        } else {
            LOG_ERR("Config file: invalid key: " <<key);
            return 1; // invalid key
        }
    }
};
