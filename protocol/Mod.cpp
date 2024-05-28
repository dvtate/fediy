#include <dlfcn.h>
#include <fstream>
#include <filesystem>

#include "nlohmann/json.hpp"

#include "Mod.hpp"

#include "App.hpp"

bool Mod::load_conf() {
    // Set error and log
    auto err = [this](const auto reason) {
        this->m_name = reason;
        LOG_ERR(reason);
    };

    // Load json from file
    std::ifstream ifs{g_app->m_config.m_data_dir + "/apps/" + m_id + "/module.json"};
    auto conf = nlohmann::json::parse( ifs );
    if (!conf.is_object()) {
        err(m_id + "/module.json: should be an object");
        return false;
    }

    // Get name
    if (!conf.contains("name")) {
        err(m_id + "/module.json: missing key: name");
        return false;
    } else {
        auto name = conf.at("name");
        if (!name.is_string()) {
            err(m_id + "/module.json: name should be a string");
            return false;
        }
        m_name = name.get<std::string>();
    }

    // Get version
    if (!conf.contains("version")) {
        err(m_id + "/module.json: missing key: version");
        return false;
    } else {
        auto version = conf.at("version");
        if (!version.is_string()) {
            err(m_id + "/module.json: version should be a string");
            return false;
        }
        m_version = version.get<std::string>();
    }

    // Get description
    if (!conf.contains("description")) {
        err(m_id + "/module.json: missing key: description");
        return false;
    } else {
        auto description = conf.at("description");
        if (!description.is_string()) {
            err(m_id + "/module.json: description should be a string");
            return false;
        }
        m_description = description.get<std::string>();
    }
}

bool Mod::start_module() {
    auto m = dlopen(

}


