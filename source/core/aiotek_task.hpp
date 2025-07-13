#ifndef AIOTEK_TASK_HPP
#define AIOTEK_TASK_HPP

#include <vector>
#include <thread>
#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include "aiotek_log.hpp"
#include "aiotek_timer.hpp"

namespace AIOTEK {

class TaskBase {
public:
    TaskBase(const std::string& name, int id)
        : name_(name), id_(id), running_(false) {}
    virtual ~TaskBase() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() const { return running_; }
    std::string name() const { return name_; }
    int id() const { return id_; }

protected:
    std::string name_;
    int id_;
    std::atomic<bool> running_;
    std::thread thread_;
    mutable std::mutex mutex_;
};

class ManagersTask {
public:
    ManagersTask();
    ~ManagersTask();
    bool start();
    void stop();
    bool isRunning() const;
    void addTask(std::unique_ptr<TaskBase> task);
    TaskBase* getTaskByName(const std::string& name);
    TaskBase* getTaskById(int id);
    std::vector<TaskBase*> getAllTasks();

private:
    bool running_;
    std::thread taskThread_;
    Timer timer_;
    std::vector<std::unique_ptr<TaskBase>> tasks_;
    mutable std::mutex mutex_;
    void run();
    void processManagers();
};

extern ManagersTask managers;

} // namespace AIOTEK

#endif // AIOTEK_TASK_HPP 