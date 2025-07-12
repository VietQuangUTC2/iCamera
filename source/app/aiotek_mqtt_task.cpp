#include <iostream>
#include <thread>
#include <chrono>
#include "utils/aiotek_log.hpp"
#include "common/aiotek_timer.hpp"
#include "module/network/mqtt/aiotek_mqtt.hpp"

namespace AIOTEK {

class MQTTTask {
private:
    bool running;
    std::thread taskThread;
    Timer timer;
    MQTTManager mqttManager;

public:
    MQTTTask() : running(false) {}
    
    ~MQTTTask() {
        stop();
    }
    
    bool start() {
        if (running) return true;
        
        AIOTEK_LOG_INFO("MQTTTask: Starting");
        
        MQTTManager::MQTTConfig config;
        config.broker = "broker.hivemq.com";
        config.port = 1883;
        config.clientId = "iCamera_";
        config.topic = "icamera/status";
        config.ssl = false;
        
        mqttManager.setConfig(config);
        
        mqttManager.onConnect([]() {
            AIOTEK_LOG_INFO("MQTTTask: Connected to broker");
        });
        
        mqttManager.onDisconnect([]() {
            AIOTEK_LOG_INFO("MQTTTask: Disconnected from broker");
        });
        
        mqttManager.onError([](const std::string& error) {
            AIOTEK_LOG_ERROR("MQTTTask: Error: " + error);
        });
        
        mqttManager.onMessage([](const std::string& topic, const std::string& payload) {
            AIOTEK_LOG_INFO("MQTTTask: Received message on " + topic + ": " + payload);
        });
        
        running = true;
        taskThread = std::thread(&MQTTTask::run, this);
        return true;
    }
    
    void stop() {
        if (!running) return;
        
        AIOTEK_LOG_INFO("MQTTTask: Stopping");
        running = false;
        
        if (taskThread.joinable()) {
            taskThread.join();
        }
        
        mqttManager.disconnect();
    }
    
    bool isRunning() const {
        return running;
    }

private:
    void run() {
        AIOTEK_LOG_INFO("MQTTTask: Thread started");
        timer.start();
        
        if (mqttManager.connect() != 0) {
            AIOTEK_LOG_ERROR("MQTTTask: Failed to connect to MQTT broker");
            return;
        }

        mqttManager.subscribe("icamera/command");
        mqttManager.subscribe("icamera/config");
        
        while (running) {
            sendStatusUpdate();
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        
        mqttManager.disconnect();
        
        timer.stop();
        AIOTEK_LOG_INFO("MQTTTask: Thread stopped after " + timer.getElapsedString());
    }
    
    void sendStatusUpdate() {
        static int counter = 0;
        nlohmann::json status;
        status["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        status["uptime"] = timer.getElapsedSeconds();
        status["counter"] = ++counter;
        status["status"] = "running";
        
        std::string topic = "icamera/status";
        if (mqttManager.publish(topic, status) == 0) {
            AIOTEK_LOG_DEBUG("MQTTTask: Sent status update");
        } else {
            AIOTEK_LOG_ERROR("MQTTTask: Failed to send status update");
        }
    }
};

} // namespace AIOTEK
