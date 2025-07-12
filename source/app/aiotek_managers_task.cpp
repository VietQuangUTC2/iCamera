#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <string>
#include "aiotek_log.hpp"
#include "aiotek_timer.hpp"
#include "aiotek_net_managers.hpp"
#include "aiotek_managers_task.hpp"

extern void task_sender();
extern void task_receiver();

namespace AIOTEK {

ManagersTask::ManagersTask() : running(false) {
    tasks.push_back({"Sender", task_sender});
    tasks.push_back({"Receiver", task_receiver});
}

ManagersTask::~ManagersTask() {
    stop();
}

bool ManagersTask::start() {
    if (running) return true;
    AIOTEK_LOG_INFO("ManagersTask: Starting");
    running = true;
    for (auto& task : tasks) {
        task.thread = std::thread(task.func);
        AIOTEK_LOG_INFO("ManagersTask: Started task " + task.name);
    }
    taskThread = std::thread(&ManagersTask::run, this);
    return true;
}

void ManagersTask::stop() {
    if (!running) return;
    AIOTEK_LOG_INFO("ManagersTask: Stopping");
    running = false;
    for (auto& task : tasks) {
        if (task.thread.joinable()) {
            task.thread.join();
            AIOTEK_LOG_INFO("ManagersTask: Stopped task " + task.name);
        }
    }
    if (taskThread.joinable()) {
        taskThread.join();
    }
}

bool ManagersTask::isRunning() const {
    return running;
}

void ManagersTask::run() {
    AIOTEK_LOG_INFO("ManagersTask: Thread started");
    timer.start();
    while (running) {
        processManagers();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    timer.stop();
    AIOTEK_LOG_INFO("ManagersTask: Thread stopped after " + timer.getElapsedString());
}

void ManagersTask::processManagers() {
    static int counter = 0;
    if (++counter % 100 == 0) {
        AIOTEK_LOG_DEBUG("ManagersTask: Processing managers");
    }
}

ManagersTask managers;
volatile bool g_shutdown_requested = false;

} // namespace AIOTEK

