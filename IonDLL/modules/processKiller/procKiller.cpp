#include "procKiller.h"
#include <set> // To track known processes
#include <spdlog/spdlog.h>

// Global set to keep track of existing processes
static std::set<DWORD> g_knownProcesses;
static std::set<std::string> g_whitelist = {"explorer.exe", "svchost.exe"};
// New function to monitor and kill new processes

void ProcessKiller() {  // Changed from DWORD WINAPI to void
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
                      std::string processName = pe32.szExeFile;
                      HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                      if (hProcess != NULL) {
                          if (pe32.th32ProcessID > 4 && 
                              g_whitelist.find(processName) == g_whitelist.end()) {
                              if (TerminateProcess(hProcess, 1)) {
                                  spdlog::info("Terminated process: {} (PID: {})", 
                                      pe32.szExeFile, pe32.th32ProcessID);
                              }
                          } else {
                              g_knownProcesses.insert(pe32.th32ProcessID);
                          }
                          CloseHandle(hProcess);
                      }
                  }
              } while (Process32Next(hSnapshot, &pe32));
          }
          CloseHandle(hSnapshot);
      }
      Sleep(1000);
  }
}
