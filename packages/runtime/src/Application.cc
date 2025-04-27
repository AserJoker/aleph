#include "runtime/include/Application.hpp"
#include "runtime/include/ModuleTask.hpp"
#include <iostream>
using namespace aleph;
using namespace aleph::runtime;
Application::Application() {}
Application::~Application() {}

int Application::run(int argc, char *argv[]) {
  try {
    for (int idx = 0; idx < argc; idx++) {
      _args.push_back(argv[idx]);
    }
    for (auto &module : _modules) {
      _co->create(new ModuleTask{module});
    }
    while (!_co->ready()) {
      _co->yield();
    }
    _systems.clear();
    _modules.clear();
    return 0;
  } catch (std::exception &e) {
    _systems.clear();
    _modules.clear();
    std::cerr << e.what() << std::endl;
    return -1;
  } catch (...) {
    _systems.clear();
    _modules.clear();
    std::cerr << "Unknown exception" << std::endl;
    return -1;
  }
}

Application *Application::addModule(const core::AutoPtr<Module> &module) {
  _modules.push_back(module);
  return this;
}

Application *Application::addSystem(const core::AutoPtr<System> &system) {
  _systems.push_back(system);
  return this;
}