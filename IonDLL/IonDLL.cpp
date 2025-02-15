#include "modules/module_manager.h"


int main() {
  // You can call InjectCode here if needed
  DumpMemoryInfo();
  return 0;
}

// DllMain is automatically called by the OS
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    main();
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  spdlog::info("DLLMain called with reason: {}", ul_reason_for_call);
  return TRUE;
}
