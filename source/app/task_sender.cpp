#include <string>
#include <iostream>
#include "aiotek_mailbox.hpp"
#include "aiotek_console.hpp"
#include "aiotek_managers_task.hpp"

void task_sender() {
    while (true) {
        std::string line = aiotek_console_readline("Enter command (msg <text> | signal <num> | event <name> <payload> | quit): ");
        if (line == "quit") {
            AIOTEK::g_mailbox.send({AIOTEK::TaskID::Sender, AIOTEK::TaskID::Receiver, AIOTEK::SignalEvent{0}});
            break;
        }
        if (line.rfind("msg ", 0) == 0) {
            AIOTEK::g_mailbox.send({AIOTEK::TaskID::Sender, AIOTEK::TaskID::Receiver, line.substr(4)});
        } else if (line.rfind("signal ", 0) == 0) {
            int sig = std::stoi(line.substr(7));
            AIOTEK::g_mailbox.send({AIOTEK::TaskID::Sender, AIOTEK::TaskID::Receiver, AIOTEK::SignalEvent{sig}});
        } else if (line.rfind("event ", 0) == 0) {
            size_t sp = line.find(' ', 6);
            if (sp != std::string::npos) {
                std::string name = line.substr(6, sp-6);
                std::string payload = line.substr(sp+1);
                AIOTEK::g_mailbox.send({AIOTEK::TaskID::Sender, AIOTEK::TaskID::Receiver, AIOTEK::CustomEvent{name, payload}});
            }
        } else {
            AIOTEK::g_mailbox.send({AIOTEK::TaskID::Sender, AIOTEK::TaskID::Receiver, AIOTEK::ErrorEvent{-1, "Unknown command: " + line}});
        }
        
        if (AIOTEK::g_shutdown_requested) {
            break;
        }
    }
} 