#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <wtypes.h>


// Thread Pool implementation
class ThreadPool {
private:
  std::vector<HANDLE> threads;
  std::queue<std::function<void()>> tasks;
  std::mutex queueMutex;
  std::condition_variable condition;
  bool stop;

  static DWORD WINAPI WorkerThread(LPVOID lpParam);

public:
  ThreadPool(size_t numThreads);

  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      stop = true;
    }
    condition.notify_all();

    for (HANDLE thread : threads) {
      WaitForSingleObject(thread, INFINITE);
      CloseHandle(thread);
    }
  }

  void enqueue(std::function<void()> task) {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      if (stop)
        return;
      tasks.emplace(task);
    }
    condition.notify_one();
  }
};

#endif /* THREADPOOL_H */
