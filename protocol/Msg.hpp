#pragma once

#include <string>
#include <cstdint>

#include "ModuleInfo.hpp"


enum class MsgType : uint8_t {
    // ??
};

struct MsgContactInfo {
    char* m_server{nullptr};
    char* m_user{nullptr};
    ModuleInfo m_app;

    ~MsgContactInfo() {
        free(m_server);
        free(m_user);
    }

};

struct Msg {
    static uint64_t unique_msg_id;
    uint64_t m_id;

    MsgContactInfo m_from;
    MsgContactInfo m_to;

    std::string m_payload;

    bool m_response_expected;

    Msg(
        MsgContactInfo from, 
        MsgContactInfo to,
        std::string payload,
        bool response_expected = false
    ):
        m_id(unique_msg_id++),
        m_from(from),
        m_to(to),
        m_payload(payload),
        m_response_expected(response_expected)
    {
    }

    ~Msg() = default;
};

