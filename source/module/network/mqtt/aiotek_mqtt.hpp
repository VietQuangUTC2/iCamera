#ifndef __AIOTEK_MQTT_HPP__
#define __AIOTEK_MQTT_HPP__

#include <memory>
#include <functional>
#include <nlohmann/json.hpp>
#include "aiotek_net_if.hpp"

class MQTTImplement;
class MQTTManager : public AIOTEK_NetworkInterface {
  public:
    struct MQTTConfig {
        std::string broker = "broker.hivemq.com";
        uint32_t port = 1883;
        std::string clientId = "";
        std::string username = "";
        std::string password = "";
        std::string caCertPath = "";
        std::string clientCertPath = "";
        std::string clientKeyPath = "";
        std::string topic = "";
        std::string willTopic = "";
        std::string willMessage = "";
        uint32_t keepAlive = 60;
        uint8_t willQos = 1;
        bool willRetained = false;
        bool ssl = true;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(MQTTConfig, broker, port, clientId, username, password, caCertPath, clientCertPath, clientKeyPath, topic, willTopic,
                                       willMessage, keepAlive, willQos, willRetained, ssl);
    };
    
    MQTTManager();
    ~MQTTManager();

    int32_t publish(const std::string& topic, const nlohmann::json& payload);
    int32_t subscribe(const std::string& topic);

    void setConfig(const MQTTConfig& config);
    MQTTConfig getConfig() const;
    

    int32_t loadConfigFromFile(const std::string& path);
    int32_t saveConfigToFile(const std::string& path) const;

    int32_t connect() override;
    void disconnect() override;
    int32_t send(const std::string& payload) override;
    int32_t receive(std::string& payload) override;

    bool isConnected() override;
    State getState() override;

    void onConnect(std::function<void()> callback) override;
    void onDisconnect(std::function<void()> callback) override;
    void onError(std::function<void(const std::string&)> callback) override;
    void onMessage(std::function<void(const std::string& topic, const std::string& payload)> callback) override;

  private:
    std::unique_ptr<MQTTImplement> pImplement;
};

#endif /* __AIOTEK_MQTT_HPP__ */