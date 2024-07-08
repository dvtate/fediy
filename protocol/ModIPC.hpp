//
// Created by tate on 7/5/24.
//

#pragma once

#include <string>
#include <functional>
#include <dlfcn.h>

#include "drogon/HttpController.h"

#include "globals.hpp"

#include "../modlib/fediymod.h"

// Abstract class
class ModIPC {
public:
    std::string m_ipc_uri;
    explicit ModIPC(const std::string& path): m_ipc_uri(path) {}

    virtual bool start() = 0;
    virtual bool stop() = 0;
//    virtual bool handle_request(
//            const drogon::HttpRequestPtr& req,
//            std::function<void(const drogon::HttpResponsePtr&)>&& callback
//    ) = 0;

    // IPC interface
    enum class IPCType {
        SHARED_LIBRARY,     // .so file
        SOCKET,        // unix socket connection
        NETWORK             // tcp connection
    };

    virtual IPCType ipc_type() = 0;
};

// Communicates with the module by dynamically linking
class ModDLLIPC : public ModIPC {
    void* m_dl_handle{nullptr};
public:
    explicit ModDLLIPC(std::string path): ModIPC(path) {}

    ~ModDLLIPC() {
        stop();
    }

    bool stop() override {
        if (m_dl_handle == nullptr)
            return true;
        auto ret = dlclose(m_dl_handle);
        if (ret == 0) {
            m_dl_handle = nullptr;
        } else {
            LOG_ERR("dlclose() gave" << ret <<": " <<dlerror());
        }
        return ret == 0;
    }

    bool start() override {
        if (m_dl_handle != nullptr) {
            DEBUG_LOG("Handle replaced!");
        }
        m_dl_handle = dlopen(m_ipc_uri.c_str(), RTLD_LAZY | RTLD_LOCAL);
        if (m_dl_handle == nullptr)
            return false;
        // load symbols
        // call start function
        return m_dl_handle != nullptr;
    }

    IPCType ipc_type() {
        return IPCType::SHARED_LIBRARY;
    }
};

class ModNetIPC : public ModIPC {
public:

    IPCType ipc_type() {
        return IPCType::NETWORK;
    }
};

class ModSockIPC : public ModIPC {
public:

    IPCType ipc_type() {
        return IPCType::SOCKET;
    }
};