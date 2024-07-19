#include <dlfcn.h>
#include <fstream>
#include <filesystem>

#include "nlohmann/json.hpp"

#include "Mod.hpp"

#include "App.hpp"

[[nodiscard]] inline std::filesystem::path Mod::appdir() const {
    return g_app->m_config.m_data_dir + "/apps/" + m_id;
}

Mod::Mod(std::string id) {
    m_id = std::move(id);
    m_path = m_id;

    // Set error and log
    auto err = [this, &id](auto reason) {
        m_error += reason;
        m_error += '\n';
        m_loaded = false;
        LOG_ERR(m_error);
    };

    // Load json from file
    std::filesystem::path mp = appdir();
    if (!std::filesystem::exists(mp / "module.json")) {
        err("missing module.json");
        return;
    }
    std::ifstream ifs{ mp / "module.json"};
    auto conf = nlohmann::json::parse( ifs );
    if (!conf.is_object()) {
        err("module.json: should be an object");
        return;
    }

    // Get routing path
    if (conf.contains("path")) {
        auto path = conf.at("path");
        if (!path.is_string()) {
            err("module.json: \"path\" should contain a uri component string");
        }
        // TODO validate uri component
        m_path = path.get<std::string>();
    }

    // Get name
    if (!conf.contains("name")) {
        err("module.json: missing key: name");
    } else {
        auto name = conf.at("name");
        if (!name.is_string()) {
            err("module.json: \"name\" should be a string");
        }
        m_name = name.get<std::string>();
    }

    // Get version
    if (!conf.contains("version")) {
        err("module.json: missing key: version");
    } else {
        auto version = conf.at("version");
        if (!version.is_string()) {
            err("module.json: \"version\" should be a string of format NN.nn");
        }
        m_version = Mod::Version(version.get<std::string>());
    }

    // Get description
    if (!conf.contains("description")) {
        err("module.json: missing key: description");
    } else {
        auto description = conf.at("description");
        if (!description.is_string()) {
            err("module.json: \"description\" should be a string");
        }
        m_description = description.get<std::string>();
    }

    // Set up IPC (order matters)
    std::string ipc_uri;
    if (conf.contains("ipc_uri")) {
        auto p = conf.at("ipc_uri");
        if (!p.is_string()) {
            err("module.json: \"ipc_uri\" should be a string");
        }
        ipc_uri = p.get<std::string>();
    }
    if (!conf.contains("ipc")) {
        err("module.json: missing key: ipc");
    } else {
        auto t = conf.at("ipc");
        if (!t.is_string()) {
            err("module.json: \"ipc\" should be either \"shared_object\", \"socket\" or \"tcp\"");
        }

        auto ts = t.get<std::string>();
        if (ts == "tcp") {
            if (ipc_uri.empty()) {
                err("module.json: \"ipc_uri\" must be defined when \"ipc\" is set to \"tcp\"");
            } else {
                m_ipc = std::make_unique<ModNetIPC>(this, ipc_uri);
            }
        } else if (ts == "shared_object") {
            if (ipc_uri.empty())
                ipc_uri = mp / "module.so";
            m_ipc = std::make_unique<ModDLLIPC>(this, ipc_uri);
        } else if (ts == "socket") {
            if (ipc_uri.empty())
                ipc_uri = mp / "ipc.socket";
//            m_ipc = std::make_unique<ModSockIPC>(ipc_uri);
        }
    }

    if (conf.contains("daemon")) {
        auto d = conf.at("daemon");
        if (!d.is_string()) {
            err("module.json: \"daemon\" should be a string path to a program");
        } else {
            m_daemon = d.get<std::string>();
            if (!m_daemon.empty() && std::filesystem::exists(m_daemon)) {
                err("module.json: \"daemon\" set to path that does not exist: " + m_daemon);
                m_daemon = "";
            }
        }
    }

    if (conf.contains("enabled")) {
        auto enabled = conf.at("enabled");
        if (enabled.is_boolean()) {
            m_enabled = enabled.get<bool>();
        } else if (enabled.is_number()) {
            m_enabled = enabled.get<double>() != 0;
        } else {
            err("module.json: \"enabled\" should be a boolean");
        }
    } else {
        m_enabled = false;
    }

    if (conf.contains("icon")) {
        auto icon = conf.at("icon");
        if (!icon.is_string()) {
            err("module.json: \"icon\" should be a string containing the icon file name");
        }
        m_icon = std::string(mp) + "/" + icon.get<std::string>();
    }

    // Get install ts
    try {
        m_install_ts = last_write_time(mp);
    } catch (const std::filesystem::filesystem_error& e) {
        LOG_ERR("Mod::load: failed to get fs::last_write_time" <<e.what());
    }

    m_loaded = true;
}

Mod::~Mod() {
    stop();
}

bool Mod::start() {
    std::lock_guard lock(m_mtx);
    return m_running = m_ipc->start();
}

bool Mod::stop() {
    std::lock_guard lock(m_mtx);
    m_running = false;
    return m_ipc->stop();
}

std::string Mod::json() {
    nlohmann::json json = {
        // Identifier speficying the protocol the app implemets
        // Multiple apps can have the same id only if they're compatible
        //      ie - chat.v3
        { "id", m_id },

        // This is the path that the users can use to access the mod (locally unique)
        { "path", m_path },

        // App version of the form X.y or just X
        { "version", m_version.str() },

        // User-readable name for the app
        { "name", m_name },

        // User-readable description of the app and what it does
        { "description", m_description },

        // App icon
        { "icon", m_icon },

        // User can disable apps they don't want
        { "enabled", m_enabled },

        //
        { "ipc", m_ipc->ipc_type() == ModIPC::IPCType::NETWORK
            ? "tcp" : m_ipc->ipc_type() == ModIPC::IPCType::SHARED_LIBRARY
            ? "shared_object" : "socket" },
        { "ipc_uri", m_ipc->m_ipc_uri },
        { "daemon", m_daemon },
    };
    return json.dump();
}

std::string Mod::user_json() {
    nlohmann::json json = {
        { "id", m_id },
        { "path", m_path },
        { "version", m_version.str() },
        { "name", m_name },
        { "description", m_description },
        { "icon", m_icon },
        { "status", status() }, // TODO give string instead
        { "loaded", m_loaded },
        { "enabled", m_enabled },
        { "running", m_running },
        { "error", m_error },
    };
    return json.dump();
}

void Mod::save() {
    // Mutex should be locked before this operation
    std::ofstream out(appdir() / "module.json");
    out <<json();
    out.close();
}

void Mod::set_enabled(bool enabled) {
    std::lock_guard lock(m_mtx);
    if (m_enabled == enabled)
        return;
    m_enabled = enabled;
    save();
}

void Mod::set_path(const std::string& path) {
    std::lock_guard lock(m_mtx);
    m_path = path;
    save();
}