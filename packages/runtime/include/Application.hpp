#pragma once
#include "core/include/Object.hpp"
#include <string>
#include <vector>
namespace aleph::runtime {
class Application : public core::Object {
private:
  std::vector<std::string> _args;

  bool _running;

public:
  Application(int argc, char *argv[]);

  ~Application() override;

  int run();

  void exit();
};
} // namespace aleph::runtime