#include "test_module.h"

// Custom function to be called after DLL injection
extern "C" __declspec(dllexport) bool DumpMemoryInfo() {
  static bool hasDumped = false;
  if (hasDumped) {
    return false;
  }
  hasDumped = true;

  // Print bytes in the app that it was injected to
  unsigned char *baseAddress = (unsigned char *)GetModuleHandle(NULL);
  MEMORY_BASIC_INFORMATION mbi;
  VirtualQuery(baseAddress, &mbi, sizeof(mbi));

  spdlog::info("Base Address: {0:x}", (uintptr_t)mbi.BaseAddress);
  spdlog::info("Allocation Base: {0:x}", (uintptr_t)mbi.AllocationBase);
  spdlog::info("Region Size: {} bytes", mbi.RegionSize);
  spdlog::info("State: {}", mbi.State == MEM_COMMIT    ? "Committed"
                            : mbi.State == MEM_RESERVE ? "Reserved"
                                                       : "Free");
  spdlog::info("Protect: {}", mbi.Protect);
  spdlog::info("Type: {}", mbi.Type == MEM_IMAGE    ? "Image"
                           : mbi.Type == MEM_MAPPED ? "Mapped"
                                                    : "Private");
  spdlog::info("You were hijacked!");
  return true;
}

