#ifndef __AIOTEK_AUDIO_HPP__
#define __AIOTEK_AUDIO_HPP__

#include <string>
#include <vector>
#include <memory>

namespace AIOTEK {

class AudioDevice {
public:
    virtual ~AudioDevice() = default;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual bool isInitialized() const = 0;
    
    virtual bool startCapture() = 0;
    virtual void stopCapture() = 0;
    virtual bool isCapturing() const = 0;
    
    virtual bool startPlayback() = 0;
    virtual void stopPlayback() = 0;
    virtual bool isPlaying() const = 0;
    
    virtual int getSampleRate() const = 0;
    virtual int getChannels() const = 0;
    virtual int getBitsPerSample() const = 0;
};

class AudioManager {
private:
    std::unique_ptr<AudioDevice> device;
    bool initialized;

public:
    AudioManager();
    ~AudioManager();
    
    bool initialize();
    void shutdown();
    bool isInitialized() const;
    
    bool startCapture();
    void stopCapture();
    bool isCapturing() const;
    
    bool startPlayback();
    void stopPlayback();
    bool isPlaying() const;
    
    bool processAudio(const std::vector<uint8_t>& data);
    std::vector<uint8_t> getAudioData();
    
    void setSampleRate(int sampleRate);
    void setChannels(int channels);
    void setBitsPerSample(int bitsPerSample);
};

} // namespace AIOTEK

#endif /* __AIOTEK_AUDIO_HPP__ */
