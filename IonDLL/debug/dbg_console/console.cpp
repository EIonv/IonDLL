#include "console.h"

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