#pragma once

#include <string>
#include <fstream>
#include <cctype>
#include <cinttypes>
#include <filesystem>
#include <cstring>

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
};


class AppConfig : public Config {
public:
    AppConfig() = default;
    virtual ~AppConfig() = default;

    /// Where files are stored
    std::string m_data_dir{"/opt/fediy/data"};

protected:
    virtual int set_key(const char* section, const char* key, const char* value) override {
        // No section
        if (section[0] != '\0')
            return false;

        // Where files are stored
        if (strcmp(key, "data_dir") == 0) {
            std::error_code ec;
            if (!std::filesystem::is_directory(value, ec)) {
                LOG_ERR("Config file: data_dir invalid path: " <<value <<std::endl);
            }
            if (ec.value() != 0) {
                LOG_ERR("filesystem::is_directory failed: " <<ec.message() <<std::endl);
            }
            m_data_dir = value;
            return 0;
        } else {
            LOG_ERR("Config file: invalid key: " <<key);
            return 1; // invalid key
        }
    }
};
