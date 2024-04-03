#pragma once
#include "Auto.hpp"
#include "Object.hpp"
#include <chrono>
#include <functional>
#include <list>
#include <thread>
namespace aleph::core {
using namespace std::chrono;
class EventLoop : public Object {
private:
  struct Task {
    std::function<void(Auto<Object> self)> handle;
    Auto<Object> self;
    system_clock::time_point start;
    system_clock::duration timeout;
    uint32_t identity;
  };
  std::list<Task> tasks;

public:
  void run() {
    while (tasks.size()) {
      auto task = *tasks.begin();
      tasks.erase(tasks.begin());
      if (task.start + task.timeout <= system_clock::now()) {
        task.handle(task.self);
      }
      else{
        tasks.push_back(task);
      }
      std::this_thread::sleep_for(4ms);
    }
  }
  template <class T>
  int setTimeout(const Auto<T> &obj, void (T::*handle)(),
                 const system_clock::duration timeout = 0ms) {
    static uint32_t identity = 0;
    tasks.push_back({[=](Auto<Object> self) -> void {
                       ((self.cast<T>().get())->*handle)();
                     },
                     obj, system_clock::now(), timeout, ++identity});
    return 0;
  }
  void clearTimeout(const uint32_t &identity) {
    for (auto it = tasks.begin(); it != tasks.end(); it++) {
      if (it->identity == identity) {
        tasks.erase(it);
        return;
      }
    }
  }
};
} // namespace aleph::core
