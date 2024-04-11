#pragma once

#include <functional>
#include <algorithm>
#include <cinttypes>
#include <string>
#include <optional>
#include <vector>

#include "ModuleInfo.hpp"
#include "Msg.hpp"

/**
 * Apps we're supporting
 */
class Module {
public:
    // Basic module id info for checking message compatibility
    ModuleInfo m_info;

    explicit Module(const std::string& path);

    void start();

    void stop();

    // Update message: reverse to/from and update payload
    bool send_message(Msg& msg);

protected:
    // Message Queue
    std::vector<Msg> m_queue;

    // .so file
    std::string m_binary;

};