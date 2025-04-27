#pragma once
#include "Module.hpp"
#include "core/include/AutoPtr.hpp"
#include "core/include/Task.hpp"
namespace aleph::runtime {
class ModuleTask : public core::Task {
private:
  core::AutoPtr<Module> _module;

public:
  ModuleTask(const core::AutoPtr<Module> &module) : _module(module) {}

  void run() override { _module->run(); }
};
}; // namespace aleph::runtime