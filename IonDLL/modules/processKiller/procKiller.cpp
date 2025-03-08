#include "procKiller.h"
#include <set>        // To track known processes
#include <spdlog/spdlog.h>

// Global set to keep track of existing processes
static std::set<DWORD> g_knownProcesses;

// New function to monitor and kill new processes
DWORD WINAPI ProcessKillerThread(LPVOID lpParam) {
  // Initial process snapshot
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot != INVALID_HANDLE_VALUE) {
      PROCESSENTRY32 pe32{sizeof(pe32)};
      if (Process32First(hSnapshot, &pe32)) {
          do {
              g_knownProcesses.insert(pe32.th32ProcessID);
          } while (Process32Next(hSnapshot, &pe32));
      }
      CloseHandle(hSnapshot);
  }

  while (true) {
      hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
      if (hSnapshot != INVALID_HANDLE_VALUE) {
          PROCESSENTRY32 pe32{sizeof(pe32)};
          if (Process32First(hSnapshot, &pe32)) {
              do {
                  if (g_knownProcesses.find(pe32.th32ProcessID) == g_knownProcesses.end()) {
                      // New process found - attempt to terminate it
                      HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                      if (hProcess != NULL) {
                          // Add basic safeguard - don't kill system processes
                          if (pe32.th32ProcessID > 4) { // Skip System Idle (0) and System (4)
                              TerminateProcess(hProcess, 1);
                              spdlog::info("Terminated new process: {} (PID: {})", 
                                  pe32.szExeFile, pe32.th32ProcessID);
                          }
                          CloseHandle(hProcess);
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