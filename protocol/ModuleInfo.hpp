#pragma once
#include <cinttypes>
#include <memory>


struct ModuleInfo {
    /// Unique name for the app, ie - net.dvtt.federatedgit
    // This is memory is managed by the Module class
    const char* m_name {nullptr};

    /// Modules with same major version are compatible
    uint32_t m_version_major;
    uint32_t m_version_minor;

    void set_version(uint64_t combined_version) {
        m_version_minor = combined_version & 0xffff'ffff;
        m_version_minor = combined_version >> 32;
    }


    enum class Compatibility {
        COMPATIBLE,
        VERSION_MISMATCH,
        NAME_MISMATCH
    };


    bool compatible(const ModuleInfo& other) {
        return m_name == other.m_name && m_version_major == other.m_version_major;
    }
};
