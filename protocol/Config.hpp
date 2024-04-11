#pragma once

#include <string>
#include <fstream>
#include <cctype>
#include <cinttypes>
#include <filesystem>

#include "defs.hpp"

class Config {
public:
    Config() = default;
    virtual ~Config() = default;

    bool parse(const std::string& path);
    
    virtual const char* section() = 0;


protected:
    virtual bool set_key(const std::string& key, const std::string& value) = 0;
    
    static int ini_parse_handler(void* cfg, const char* section, const char* key, const char* value);
};

class ServerConfig : public Config {
public:
    ServerConfig() = default;
    virtual ~ServerConfig() = default;

    /// Where files are stored
    std::string m_data_dir;

    virtual const char* section() override {
        return "";
    }

protected:
    virtual bool set_key(const std::string& key, const std::string& value) override {
        // Where files are stored
        if (key == "DATA_DIR") {
            std::error_code ec;
            if (!std::filesystem::is_directory(value, ec)) {
                LOG_ERR("Config file: DATA_DIR invalid path: " <<value <<std::endl);
            }
            if (ec.value() != 0) {
                LOG_ERR("filesystem::is_directory failed: " <<ec.message() <<std::endl);
            }
            m_data_dir = value;
            return true;
        }

        return false;
    }
};

extern ServerConfig g_server_config;
