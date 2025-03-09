#ifndef CONSOLE_H
#define CONSOLE_H

#include <Windows.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>  // Added for file logging
#include <fmt/format.h>
#include <string>

void InitializeLogging();

// Debug Console Manager class
class DebugConsole {
  private:
      FILE* consoleFile = nullptr;
      bool isEnabled = false;
  
  public:
      DebugConsole(bool enable = false) : isEnabled(enable) {}
      
      ~DebugConsole() {
          Cleanup();
      }
  
      bool Initialize();
  
      void Cleanup();
  
      bool IsEnabled() const { return isEnabled; }
  };

#endif /* CONSOLE_H */
