#pragma once
#include "core/include/AutoPtr.hpp"
#include "core/include/Co.hpp"
#include "core/include/Promise.hpp"
#include "core/include/Singleton.hpp"
#include "core/include/Task.hpp"
namespace aleph::core {
class Async {
private:
  template <class T>
  static AutoPtr<Task> createTask(const core::AutoPtr<Promise<T>> &promise,
                                  auto &&entry, auto &&...args) {
    return new FunctionalTask{[=]() {
      AutoPtr pro = promise;
      pro->resolve(entry(args...));
    }};
  }
  static AutoPtr<Task> createTask(const core::AutoPtr<Promise<void>> &promise,
                                  auto &&entry, auto &&...args) {
    return new FunctionalTask{[=]() {
      AutoPtr pro = promise;
      entry(args...);
      pro->resolve();
    }};
  }

public:
  Async() = delete;

  template <class Fn, class... Args>
  static auto run(Fn &&entry, Args &&...args) {
    AutoPtr promise = new Promise<decltype(entry(args...))>{};
    Singleton<Co>::get()->create(createTask(promise, entry, args...));
    return promise;
  }
};
} // namespace aleph::core