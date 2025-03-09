#include "console.h"
#include <stdexcept>
#include <iostream>

// Helper function to get temp directory path
std::string GetTempDir() {
  char tempPath[MAX_PATH];
  GetTempPathA(MAX_PATH, tempPath);
  return std::string(tempPath);
}

// Initialize logging with console and file sinks
void InitializeLogging() {
  try {
    // Create vector of sinks
    std::vector<spdlog::sink_ptr> sinks;
    
    // Console sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    sinks.push_back(console_sink);
    
    // File sink in temp directory
    std::string logFilePath = GetTempDir() + "debug_log_" + 
                             std::to_string(std::time(nullptr)) + ".txt";
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
    file_sink->set_level(spdlog::level::info);
    sinks.push_back(file_sink);
    
    // Create and configure logger
    const char *logPattern = "[%Y-%m-%d %H:%M:%S.%e][%^%l%$] %v";
    auto logger = std::make_shared<spdlog::logger>("ion", sinks.begin(), sinks.end());
    spdlog::set_default_logger(logger);
    spdlog::set_pattern(logPattern);
    spdlog::set_level(spdlog::level::info);
    
    // Log initialization messages
    spdlog::info("Logging initialized successfully");
    spdlog::info("Debug log file created at: {}", logFilePath);
    
  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
  }
}

bool DebugConsole::Initialize() {
  if (!isEnabled)
    return true;

  // Allocate console
  if (!AllocConsole()) {
    spdlog::error("Failed to allocate console");
    return false;
  }

  // Redirect stdout
  if (freopen_s(&consoleFile, "CONOUT$", "w", stdout) != 0) {
    spdlog::error("Failed to redirect stdout to console");
    FreeConsole();
    return false;
  }

  // Redirect stderr
  if (freopen_s(&consoleFile, "CONOUT$", "w", stderr) != 0) {
    spdlog::error("Failed to redirect stderr to console");
    FreeConsole();
    return false;
  }

  // Set console title
  SetConsoleTitle(
      fmt::format("IonDLL - PID: {}", GetCurrentProcessId()).c_str());

  // Configure spdlog pattern
  spdlog::set_pattern("[%H:%M:%S.%e][%^%l%$] %v");

  spdlog::info("Console initialized successfully");
  return true;
}

void DebugConsole::Cleanup() {
  if (!isEnabled)
    return;

  if (consoleFile) {
    fclose(consoleFile);
    consoleFile = nullptr;
  }
  FreeConsole();
}