#pragma once
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include "aiotek_task.hpp"

namespace AIOTEK {

class MQTTTask : public TaskBase {
public:
    MQTTTask(int id);
    ~MQTTTask();
    void start() override;
    void stop() override;
    bool isRunning() const override;

private:
    void threadFunc();
};

} // namespace AIOTEK 