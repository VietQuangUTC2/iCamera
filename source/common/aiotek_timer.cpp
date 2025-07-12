#include "aiotek_timer.hpp"
#include <sstream>
#include <iomanip>

namespace AIOTEK {

Timer::Timer() : isRunning(false) {
}

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    isRunning = true;
}

void Timer::stop() {
    if (isRunning) {
        endTime = std::chrono::high_resolution_clock::now();
        isRunning = false;
    }
}

void Timer::reset() {
    isRunning = false;
}

double Timer::getElapsedSeconds() const {
    auto end = isRunning ? std::chrono::high_resolution_clock::now() : endTime;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startTime);
    return duration.count() / 1000000.0;
}

double Timer::getElapsedMilliseconds() const {
    auto end = isRunning ? std::chrono::high_resolution_clock::now() : endTime;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startTime);
    return duration.count() / 1000.0;
}

double Timer::getElapsedMicroseconds() const {
    auto end = isRunning ? std::chrono::high_resolution_clock::now() : endTime;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startTime);
    return static_cast<double>(duration.count());
}

bool Timer::running() const {
    return isRunning;
}

std::string Timer::getElapsedString() const {
    double seconds = getElapsedSeconds();
    
    std::stringstream ss;
    if (seconds < 1.0) {
        ss << std::fixed << std::setprecision(3) << (seconds * 1000.0) << "ms";
    } else if (seconds < 60.0) {
        ss << std::fixed << std::setprecision(3) << seconds << "s";
    } else {
        int minutes = static_cast<int>(seconds / 60.0);
        double remainingSeconds = seconds - (minutes * 60.0);
        ss << minutes << "m " << std::fixed << std::setprecision(1) << remainingSeconds << "s";
    }
    
    return ss.str();
}

} // namespace AIOTEK
