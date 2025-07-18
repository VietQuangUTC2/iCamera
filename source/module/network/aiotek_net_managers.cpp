#include "aiotek_log.hpp"
#include "aiotek_net_managers.hpp"

std::unordered_map<std::string, std::unique_ptr<AIOTEK_NetworkInterface>> managers;
std::mutex managers_mutex;

namespace AIOTEK_NetworkManagers {

NetworkManagers::NetworkManagers() {
    AIOTEK_LOG_INFO("NetworkManagers: Initialized");
}

NetworkManagers::~NetworkManagers() {
    AIOTEK_LOG_INFO("NetworkManagers: Shutting down");
    
    std::lock_guard<std::mutex> lock(managers_mutex);
    managers.clear();
}

} // namespace AIOTEK_NetworkManagers
