#pragma once

#include <functional>
#include <algorithm>
#include <cinttypes>
#include <string>

#include "ModuleInfo.hpp"
#include "Msg.hpp"

/**
 * Apps we're supporting
 */
class Module {
public:
    // Basic module id info for checking message compatibility
    ModuleInfo m_info;

    void start();

    void stop();

    void send_message(Msg msg);

};