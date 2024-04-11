#pragma once
#include <cinttypes>

struct ModuleInfo {
    /// Unique name for the app, ie - net.dvtt.federatedgit
    /// This name comes from a map so does not have to be freed
    const char* name;

    /// Modules with same major version are compatible
    uint32_t version_major;
    uint32_t version_minor;
};
