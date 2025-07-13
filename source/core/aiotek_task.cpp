#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <string>
#include "aiotek_log.hpp"
#include "aiotek_timer.hpp"
#include "aiotek_net_managers.hpp"
#include "aiotek_task.hpp"
#include "aiotek_mqtt_task.hpp"


using namespace AIOTEK;
extern void task_sender();
extern void task_receiver();
extern void task_mqtt();

namespace AIOTEK {

ManagersTask managers;

ManagersTask::ManagersTask() : running_(false) {
    addTask(std::make_unique<MQTTTask>(1));
}

ManagersTask::~ManagersTask() {
    stop();
}

bool ManagersTask::start() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (running_)
        return true;
    AIOTEK_LOG_INFO("ManagersTask: Starting");
    running_ = true;
    for (auto& task : tasks_) {
        task->start();
        AIOTEK_LOG_INFO("ManagersTask: Started task " + task->name());
    }
    taskThread_ = std::thread(&ManagersTask::run, this);
    return true;
}

void ManagersTask::stop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_)
        return;
    AIOTEK_LOG_INFO("ManagersTask: Stopping");
    running_ = false;
    for (auto& task : tasks_) {
        task->stop();
        AIOTEK_LOG_INFO("ManagersTask: Stopped task " + task->name());
    }
    if (taskThread_.joinable()) {
        taskThread_.join();
    }
}

bool ManagersTask::isRunning() const {
    return running_;
}

void ManagersTask::addTask(std::unique_ptr<TaskBase> task) {
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_.push_back(std::move(task));
}

TaskBase* ManagersTask::getTaskByName(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& task : tasks_) {
        if (task->name() == name) return task.get();
    }
    return nullptr;
}

TaskBase* ManagersTask::getTaskById(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& task : tasks_) {
        if (task->id() == id) return task.get();
    }
    return nullptr;
}

std::vector<TaskBase*> ManagersTask::getAllTasks() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<TaskBase*> result;
    for (auto& task : tasks_) {
        result.push_back(task.get());
    }
    return result;
}

void ManagersTask::run() {
    AIOTEK_LOG_INFO("ManagersTask: Thread started");
    timer_.start();
    while (running_) {
        processManagers();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    timer_.stop();
    AIOTEK_LOG_INFO("ManagersTask: Thread stopped after " + timer_.getElapsedString());
}

void ManagersTask::processManagers() {
    static int counter = 0;
    if (++counter % 100 == 0) {
        AIOTEK_LOG_DEBUG("ManagersTask: Processing managers");
    }
}

} // namespace AIOTEK
