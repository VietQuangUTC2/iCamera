#include <iostream>
#include "aiotek_mailbox.hpp"
#include "aiotek_task.hpp"
#include "aiotek_console.hpp"

void task_receiver() {
    while (true) {
        auto env = AIOTEK::g_mailbox.receive();
        std::cout << "[Receiver] From: " << static_cast<int>(env.sender)
                  << " To: " << static_cast<int>(env.receiver) << std::endl;
        std::visit([](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, AIOTEK::SignalEvent>) {
                std::cout << "[Receiver] SignalEvent: " << arg.signal << std::endl;
                if (arg.signal == 0) {
                    std::cout << "[Receiver] Shutdown signal received. Exiting..." << std::endl;
                    g_shutdown_requested = true;
                }
            } else if constexpr (std::is_same_v<T, AIOTEK::CustomEvent>) {
                std::cout << "[Receiver] CustomEvent: " << arg.name << " | " << arg.payload << std::endl;
            } else if constexpr (std::is_same_v<T, AIOTEK::ErrorEvent>) {
                std::cout << "[Receiver] ErrorEvent: " << arg.code << " | " << arg.message << std::endl;
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "[Receiver] String: " << arg << std::endl;
            } else if constexpr (std::is_same_v<T, int>) {
                std::cout << "[Receiver] Int: " << arg << std::endl;
            }
        }, env.payload);
        
        if (g_shutdown_requested) {
            break;
        }
    }
} 