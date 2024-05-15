#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "ModuleInfo.hpp"


enum class MsgType : uint8_t {
    // ??
};

struct MsgOrigin {
    char* m_server{nullptr};
    char* m_user{nullptr};
    ModuleInfo m_app;

    ~MsgOrigin() {
        free(m_server);
        free(m_user);
    }

};


/**
 * Message between users, apps, servers, etc.
 */
struct Msg {
    static uint64_t unique_msg_id;
    uint64_t m_id;

    MsgOrigin m_orig;
    ModuleInfo m_dest;

    std::string m_payload;

    bool m_response_expected;

    Msg(
        MsgOrigin origin, 
        ModuleInfo destination,
        std::string payload,
        bool response_expected = false
    ):
        m_id(unique_msg_id++),
        m_orig(std::move(origin)),
        m_dest(std::move(destination)),
        m_payload(std::move(payload)),
        m_response_expected(std::move(response_expected))
    {
    }

    ~Msg() = default;
};

/**
 * For a reply, the message orig+dest are used in opposite places and m_payload is overwritten 
 */
using Reply = Msg;