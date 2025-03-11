#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


// Thread pool class named Watcher
class Thread_Pool {
public:
  Thread_Pool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
      workers.emplace_back([this] {
        while (true) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty())
              return;
            task = std::move(tasks.front());
            tasks.pop();
          }
          task();
        }
      });
    }
  }

  ~Thread_Pool() {
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
      tasks.push(std::move(task));
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