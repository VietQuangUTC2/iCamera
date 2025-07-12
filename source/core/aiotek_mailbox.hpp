#pragma once
#include <variant>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <iostream>

namespace AIOTEK {

enum class TaskID {
    Unknown = 0,
    Console,
    Sender,
    Receiver,
    Audio,
    Video,
    Managers,
    MQTT,
};

inline const char* TaskIDToString(TaskID id)
{
    switch (id) {
        case TaskID::Unknown:
            return "Unknown";
        case TaskID::Console:
            return "Console";
        case TaskID::Sender:
            return "Sender";
        case TaskID::Receiver:
            return "Receiver";
        case TaskID::Audio:
            return "Audio";
        case TaskID::Video:
            return "Video";
        case TaskID::Managers:
            return "Managers";
        case TaskID::MQTT:
            return "MQTT";
        default:
            return "(invalid)";
    }
}

inline void PrintAllTaskIDs()
{
    std::cout << "List of TaskID:" << std::endl;
    std::cout << static_cast<int>(TaskID::Unknown) << ": " << TaskIDToString(TaskID::Unknown) << std::endl;
    std::cout << static_cast<int>(TaskID::Console) << ": " << TaskIDToString(TaskID::Console) << std::endl;
    std::cout << static_cast<int>(TaskID::Sender) << ": " << TaskIDToString(TaskID::Sender) << std::endl;
    std::cout << static_cast<int>(TaskID::Receiver) << ": " << TaskIDToString(TaskID::Receiver) << std::endl;
    std::cout << static_cast<int>(TaskID::Audio) << ": " << TaskIDToString(TaskID::Audio) << std::endl;
    std::cout << static_cast<int>(TaskID::Video) << ": " << TaskIDToString(TaskID::Video) << std::endl;
    std::cout << static_cast<int>(TaskID::Managers) << ": " << TaskIDToString(TaskID::Managers) << std::endl;
    std::cout << static_cast<int>(TaskID::MQTT) << ": " << TaskIDToString(TaskID::MQTT) << std::endl;
}

enum class EventType { None, Signal, Shutdown, Error, Custom };

struct SignalEvent {
    int signal;
};

struct ErrorEvent {
    int code;
    std::string message;
};

struct CustomEvent {
    std::string name;
    std::string payload;
};

using MailboxMessage = std::variant<SignalEvent, ErrorEvent, CustomEvent, std::string, int>;

struct MailboxEnvelope {
    TaskID sender;
    TaskID receiver;
    MailboxMessage payload;
};

class Mailbox {
  public:
    void send(const MailboxEnvelope& env);
    MailboxEnvelope receive();
    std::optional<MailboxEnvelope> try_receive();

  private:
    std::queue<MailboxEnvelope> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

extern Mailbox g_mailbox;

} // namespace AIOTEK
