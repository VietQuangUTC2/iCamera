#include "aiotek_mqtt_task.hpp"
#include <iostream>
#include <chrono>

namespace AIOTEK {

MQTTTask::MQTTTask(int id)
    : TaskBase("MQTT", id) {}

MQTTTask::~MQTTTask() {
    stop();
}

void MQTTTask::start() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (running_) return;
    running_ = true;
    thread_ = std::thread(&MQTTTask::threadFunc, this);
    AIOTEK_LOG_INFO("MQTTTask: Started");
}

void MQTTTask::stop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) return;
    running_ = false;
    if (thread_.joinable()) thread_.join();
    AIOTEK_LOG_INFO("MQTTTask: Stopped");
}

bool MQTTTask::isRunning() const {
    return running_;
}

void MQTTTask::threadFunc() {
    AIOTEK_LOG_INFO("MQTTTask: Thread running");
    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        AIOTEK_LOG_DEBUG("MQTTTask: Simulating work...");
    }
    AIOTEK_LOG_INFO("MQTTTask: Thread exiting");
}

} // namespace AIOTEK 