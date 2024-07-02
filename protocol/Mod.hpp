#pragma once

#include <string>
#include <filesystem>

#include "globals.hpp"

// TODO abstract base with child classes for each method of sending requests to the modules

class ModMgr;

class Mod {
public:
    std::mutex m_mtx;


public:
    // Metadata
    std::string m_id;
    std::string m_name;
    std::string m_version;
    std::string m_description;
    std::filesystem::file_time_type m_install_ts;

    // Runtime data
    bool m_loaded{true};
    bool m_enabled;
    bool m_running{false};
    std::string m_error;

    // IPC interface
    enum class IPCType {
        SHARED_LIBRARY,     // .so file
        UNIX_SOCKET,        // unix socket connection
        NETWORK             // tcp connection
    } m_ipc;
    std::string m_ipc_uri;
    void* m_dl_handle;

    Mod() = default;
    ~Mod();

    bool start();
    bool stop();

    bool set_enabled(); // update mod metadata

    static std::unique_ptr<Mod> load(const std::string& id, std::string& fail_reason);

    friend class ModMgr;
};
