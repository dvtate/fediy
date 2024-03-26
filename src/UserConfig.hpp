#pragma once

#include <string>
#include <fstream>
#include <cctype>
#include <cinttypes>
#include <filesystem>

#include "defs.hpp"
#include "validate.hpp"

class UserConfig {
public:
    /// Where files are stored
    std::string m_data_dir;

    UserConfig() = default;
    virtual ~UserConfig() = default;

    bool parse(const std::string& path) {
        // TODO use an off the shelf ini parser instead
        std::ifstream f{path};
        std::string line;
        bool ret = true;
        unsigned line_num = 0;
        while (std::getline(f, line)) {
            line_num++;

            // Skip spaces
            uint32_t end_space = 0; 
            while (end_space < line.size() && isspace(line[end_space]))
                end_space++;

            // Ignore comment or blank line
            if (end_space = line.size() || line[end_space] == '#')
                continue;

            // Find equals sign
            uint32_t eq = end_space;
            while (eq < line.size() && line[eq] != '=')
                eq++;
            if (eq == line.size()) {
                std::cerr <<"Config File: Line " <<line_num <<": Syntax error: Missing equals sign" <<std::endl;
                ret = false;
                continue;
            }

            // TODO maybe allow spaces around equals sign?
            // TODO allow comments at end

            // Add to dict
            std::string key = line.substr(end_space, eq - end_space);
            std::string value = line.substr(eq);
            if (!set_key(key, value)) {
                std::cerr <<"Config File: Line " <<line_num <<": Invalid key: `" <<key <<"`" <<std::endl;
                ret = false;
            }
        }
    }

protected:
    virtual bool set_key(const std::string& key, const std::string& value) {
        // Where files are stored
        if (key == "DATA_DIR") {
            std::error_code ec;
            if (!std::filesystem::is_directory(value, ec)) {
                std::cerr <<"Config file: DATA_DIR invalid path: " <<value <<std::endl;
            }
            if (ec.value() != 0) {
                std::cerr <<"filesystem::is_directory failed: " <<ec.message() <<std::endl;
            }
            m_data_dir = value;
            return true;
        }


        return false;
    }
};
