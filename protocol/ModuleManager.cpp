
#include "ModuleManager.hpp"


std::optional<Msg> ModuleManager::make_msg(std::string module, uint64_t version, std::string contents, bool reply_expected) {
    auto modit = m_name_map.find(module);
    if (modit == m_name_map.end())
        return std::nullopt; // module not found
    Module& m = modit->second;
    ModuleInfo origin_modinfo
    Msg msg{}
}