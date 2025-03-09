#ifndef KEYLOG_H
#define KEYLOG_H

#include <string>
#include <mutex>
#include <chrono>
#include <windows.h>

class KeyLogger {
public:
  KeyLogger(const std::string& filePath); // Constructor with file path
  ~KeyLogger();
  void StartLogging();
  std::string GetKeyName(int vkCode, bool shiftPressed, bool capsLock);

private:
  bool running;
  std::mutex logMutex;
  FILE* logFile; // File pointer for logging
};

#endif