#ifndef __AIOTEK_NET_IF_H__
#define __AIOTEK_NET_IF_H__

#include <string>
#include <functional>

class AIOTEK_NetworkInterface {
  public:
    enum class State { Disconnected, Connecting, Connected, Disconnecting };
    virtual ~AIOTEK_NetworkInterface() = default;

    virtual int32_t connect() = 0;
    virtual void disconnect() = 0;
    virtual int32_t send(const std::string& data) = 0;
    virtual int32_t receive(std::string& data) = 0;

    virtual bool isConnected() = 0;
    virtual State getState() = 0;

    virtual void onConnect(std::function<void()> callback) = 0;
    virtual void onDisconnect(std::function<void()> callback) = 0;
    virtual void onError(std::function<void(const std::string&)> callback) = 0;
    virtual void onMessage(std::function<void(const std::string& topic, const std::string& payload)> callback) = 0;
};
#endif /* __AIOTEK_NET_IF_H__ */