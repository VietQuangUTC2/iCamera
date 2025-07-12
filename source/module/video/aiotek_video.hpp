#ifndef __AIOTEK_VIDEO_HPP__
#define __AIOTEK_VIDEO_HPP__

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace AIOTEK {

struct VideoFrame {
    std::vector<uint8_t> data;
    int width;
    int height;
    int channels;
    std::string format;
    uint64_t timestamp;
};

struct VideoConfig {
    int width = 1920;
    int height = 1080;
    int fps = 30;
    std::string format = "YUYV";
    std::string device = "/dev/video0";
};

class VideoDevice {
public:
    virtual ~VideoDevice() = default;
    
    virtual bool initialize(const VideoConfig& config) = 0;
    virtual void shutdown() = 0;
    virtual bool isInitialized() const = 0;
    
    virtual bool startCapture() = 0;
    virtual void stopCapture() = 0;
    virtual bool isCapturing() const = 0;
    
    virtual VideoFrame getFrame() = 0;
    virtual bool hasFrame() const = 0;
    
    virtual VideoConfig getConfig() const = 0;
    virtual bool setConfig(const VideoConfig& config) = 0;
};

class VideoManager {
private:
    std::unique_ptr<VideoDevice> device;
    bool initialized;
    VideoConfig config;
    std::function<void(const VideoFrame&)> frameCallback;

public:
    VideoManager();
    ~VideoManager();
    
    bool initialize(const VideoConfig& config = VideoConfig{});
    void shutdown();
    bool isInitialized() const;
    
    bool startCapture();
    void stopCapture();
    bool isCapturing() const;
    
    VideoFrame getFrame();
    bool hasFrame() const;
    
    void setFrameCallback(std::function<void(const VideoFrame&)> callback);
    
    VideoConfig getConfig() const;
    bool setConfig(const VideoConfig& config);
    
    bool processFrame(const VideoFrame& frame);
    bool saveFrame(const VideoFrame& frame, const std::string& filename);
};

} // namespace AIOTEK

#endif /* __AIOTEK_VIDEO_HPP__ */
