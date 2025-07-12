#ifndef AIOTEK_MANAGERS_TASK_HPP
#define AIOTEK_MANAGERS_TASK_HPP

#include <vector>
#include <thread>
#include <functional>
#include <string>
#include "aiotek_log.hpp"
#include "aiotek_timer.hpp"

namespace AIOTEK {

struct TaskEntry {
    std::string name;
    std::function<void()> func;
    std::thread thread = std::thread();
};

class ManagersTask {
private:
    bool running;
    std::thread taskThread;
    Timer timer;
    std::vector<TaskEntry> tasks;

public:
    ManagersTask();
    ~ManagersTask();
    bool start();
    void stop();
    bool isRunning() const;

private:
    void run();
    void processManagers();
};

extern ManagersTask managers;
extern volatile bool g_shutdown_requested;

} // namespace AIOTEK

#endif // AIOTEK_MANAGERS_TASK_HPP 