#include <dlfcn.h>
#include <fstream>
#include <filesystem>

#include "nlohmann/json.hpp"

#include "Mod.hpp"

#include "App.hpp"

std::unique_ptr<Mod> Mod::load(const std::string& id, std::string& fail_reason) {
    std::unique_ptr<Mod> ret = std::make_unique<Mod>();

    // Set error and log
    auto err = [&fail_reason, &id](auto reason) {
        fail_reason = std::string("Module ") + id + ": " + reason;
        LOG_ERR(fail_reason);
    };

    // Load json from file
    std::filesystem::path mp = g_app->m_config.m_data_dir + "/apps/" + id;
    std::ifstream ifs{ mp / "module.json"};
    auto conf = nlohmann::json::parse( ifs );
    if (!conf.is_object()) {
        err("module.json: should be an object");
        return nullptr;
    }

    // Get name
    if (!conf.contains("name")) {
        err("module.json: missing key: name");
        return nullptr;
    } else {
        auto name = conf.at("name");
        if (!name.is_string()) {
            err("module.json: \"name\" should be a string");
            return nullptr;
        }
        ret->m_name = name.get<std::string>();
    }

    // Get version
    if (!conf.contains("version")) {
        err("module.json: missing key: version");
        return nullptr;
    } else {
        auto version = conf.at("version");
        if (!version.is_string()) {
            err("module.json: \"version\" should be a string");
            return nullptr;
        }
        ret->m_version = version.get<std::string>();
    }

    // Get description
    if (!conf.contains("description")) {
        err("module.json: missing key: description");
        return nullptr;
    } else {
        auto description = conf.at("description");
        if (!description.is_string()) {
            err("module.json: \"description\" should be a string");
            return nullptr;
        }
        ret->m_description = description.get<std::string>();
    }

    if (!conf.contains("ipc")) {
        err("module.json: missing key: ipc");
        return nullptr;
    } else {
        auto t = conf.at("ipc");
        if (!t.is_string()) {
            err("module.json: \"ipc\" should be either \"shared_object\", \"socket\" or \"tcp\"");
            return nullptr;
        }

        auto ts = t.get<std::string>();
        if (ts == "tcp") {
            ret->m_ipc = IPCType::NETWORK;
        } else if (ts == "shared_object") {
            ret->m_ipc = IPCType::SHARED_LIBRARY;
            ret->m_ipc_uri = mp / "module.so";
        } else if (ts == "socket") {
            ret->m_ipc = IPCType::UNIX_SOCKET;
            ret->m_ipc_uri = mp / "ipc.socket";
        }
    }
    if (conf.contains("ipc_uri")) {
        auto p = conf.at("ipc_uri");
        if (!p.is_string()) {
            err("module.json: \"ipc_uri\" should be a string");
            return nullptr;
        }
        ret->m_ipc_uri = p.get<std::string>();
    }

    // Get install ts
    try {
        ret->m_install_ts = last_write_time(mp);
    } catch (const std::filesystem::filesystem_error& e) {
        LOG_ERR("Mod::load: failed to get fs::last_write_time" <<e.what());
    }

    return std::move(ret);
}



Mod::~Mod() {
    stop();
}

bool Mod::start() {
    switch (m_ipc) {
    case Mod::IPCType::SHARED_LIBRARY:
        {
            std::string mod_path = g_app->m_config.m_data_dir + "/apps/" + m_id + "/module.so";
            m_dl_handle = dlopen(mod_path.c_str(), RTLD_LAZY | RTLD_LOCAL);
            if (m_dl_handle == nullptr)
                return false;
        }
        break;
    case Mod::IPCType::NETWORK:
    case Mod::IPCType::UNIX_SOCKET:
        std::cerr <<"Unimplemented module type!\n";
        break;
    }

//    dlsym(m_handle, "");
    return true;
}


bool Mod::stop() {
    switch (m_ipc) {
        case Mod::IPCType::SHARED_LIBRARY:
            if (m_dl_handle != nullptr) {
                auto res = dlclose(m_dl_handle);
                if (res != 0) {
                    DEBUG_LOG("failed to dlclose() handle for mod " << m_id << " : " << res << " : " << dlerror());
                }
                m_dl_handle = nullptr;
                return !res;
            }
            return true;

        case Mod::IPCType::UNIX_SOCKET:
            // TODO Disconnect
        case Mod::IPCType::NETWORK:
            // TODO Disconnect
            m_ipc_uri = "";
            return true;
    }
}