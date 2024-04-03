#include "core/Singleton.hpp"
#include "device/Terminal.hpp"
#include "framework/Application.hpp"
#include "framework/Director.hpp"
using namespace aleph;
int main(int argc, char *argv[]) {
  auto app = core::Singleton<framewrok::Application>::initialize(argc, argv);
  auto terminal = core::Singleton<device::Terminal>::initialize(
      device::TerminalMode::COLOREX);
  auto director = core::Singleton<framework::Director>::getInstance();
  app->initialize();
  terminal->initialize();
  director->initialize();
  return app->run();
}