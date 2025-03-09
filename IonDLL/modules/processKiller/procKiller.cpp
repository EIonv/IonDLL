#include "procKiller.h"
#include <set> // To track known processes
#include <spdlog/spdlog.h>

// Global set to keep track of existing processes
static std::set<DWORD> g_knownProcesses;
static std::set<std::string> g_whitelist = {"explorer.exe", "svchost.exe"};
// New function to monitor and kill new processes
DWORD WINAPI ProcessKillerThread(LPVOID lpParam) {
  // Initial process snapshot
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot != INVALID_HANDLE_VALUE) {
    PROCESSENTRY32 pe32{sizeof(pe32)};
    if (Process32First(hSnapshot, &pe32)) {
      do {
        g_knownProcesses.insert(pe32.th32ProcessID); // Add to known processes
      } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
  }
  // Main loop to monitor new processes
  while (true) {
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
      PROCESSENTRY32 pe32{sizeof(pe32)};
      if (Process32First(hSnapshot, &pe32)) {
        do {
          if (g_knownProcesses.find(pe32.th32ProcessID) ==
              g_knownProcesses.end()) {
            // New process found - check if it's in the whitelist
            if (g_whitelist.find(pe32.szExeFile) == g_whitelist.end()) {
              // Attempt to terminate it
              HANDLE hProcess =
                  OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
              if (hProcess != NULL) {
                // Add basic safeguard - don't kill system processes
                if (pe32.th32ProcessID >
                    4) { // Skip System Idle (0) and System (4)
                  TerminateProcess(hProcess, 1);
                  spdlog::info("Terminated process: {}", pe32.szExeFile);
                }
                CloseHandle(hProcess);
              }
            }
          }
        } while (Process32Next(hSnapshot, &pe32));
      }
      CloseHandle(hSnapshot);
    }
    Sleep(1000); // Check every second
  }
  return 0;
}