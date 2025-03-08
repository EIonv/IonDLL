#include "threadpool.h"

DWORD __stdcall ThreadPool::WorkerThread(LPVOID lpParam) {
  ThreadPool *pool = static_cast<ThreadPool *>(lpParam);
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(pool->queueMutex);
      if (pool->stop && pool->tasks.empty())
        return 0;

      if (pool->tasks.empty())
        pool->condition.wait(
            lock, [pool] { return !pool->tasks.empty() || pool->stop; });

      if (pool->stop && pool->tasks.empty())
        return 0;

      task = std::move(pool->tasks.front());
      pool->tasks.pop();
    }
    task();
  }
  return 0;
}

ThreadPool::ThreadPool(size_t numThreads): stop(false) {
  for (size_t i = 0; i < numThreads; ++i) {
    HANDLE thread = CreateThread(NULL, 0, WorkerThread, this, 0, NULL);
    if (thread) {
      threads.push_back(thread);
    }
  }
}
