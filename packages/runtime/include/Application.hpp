#pragma once
#include "Module.hpp"
#include "System.hpp"
#include "core/include/AutoPtr.hpp"
#include "core/include/Co.hpp"
#include "core/include/Object.hpp"
#include "core/include/Singleton.hpp"
#include <string>
#include <vector>
namespace aleph::runtime {
class Application : public core::Object {
private:
  std::vector<std::string> _args;

  core::Singleton<core::Co> _co;

  std::vector<core::AutoPtr<Module>> _modules;

  std::vector<core::AutoPtr<System>> _systems;

public:
  Application();

  ~Application() override;

  int run(int argc, char *argv[]);

  Application *addModule(const core::AutoPtr<Module> &module);

  Application *addSystem(const core::AutoPtr<System> &system);
};
} // namespace aleph::runtime