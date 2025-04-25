#pragma once
#include "core/include/ObjectBase.hpp"
#include <functional>
namespace aleph::core {
class Task : public ObjectBase {
public:
  virtual void run() = 0;
};
class FunctionalTask : public Task {
private:
  std::function<void()> _func;

public:
  FunctionalTask(const std::function<void()> &func) : _func(func) {}

  void run() override { _func(); }
};
}; // namespace aleph::core