#pragma once
#include "../core/EventBus.hpp"
#include "../core/EventLoop.hpp"
#include "../core/Object.hpp"
#include "../core/Singleton.hpp"
#include "EventInitialize.hpp"
#include "EventPostInitialize.hpp"
#include "EventPreInitialize.hpp"
#include "EventTick.hpp"
#include "EventUnInitialize.hpp"
#include <curses.h>
#include <fmt/format.h>
#include <string>
#include <vector>
namespace aleph::framewrok {
class Application : public core::Object {
private:
  std::vector<std::string> args;
  bool running;
  uint32_t exitCode;

  void onMainLoop() {
    if (!running) {
      return;
    }
    auto bus = core::Singleton<core::EventBus>::getInstance();
    bus->emit(framework::EventTick());
    auto loop = core::Singleton<core::EventLoop>::getInstance();
    loop->setTimeout<Application>(this, &Application::onMainLoop);
  }

public:
  Application(int argc, char *argv[]) : running(true), exitCode(0) {
    for (int i = 0; i < argc; i++) {
      args.push_back(argv[i]);
    }
  }

  void initialize() {
    auto loop = core::Singleton<core::EventLoop>::getInstance();
    loop->setTimeout<Application>(this, &Application::onMainLoop);
  }
  int run() {
    auto bus = core::Singleton<core::EventBus>::getInstance();
    bus->emit(framework::EventPreInitialize());
    bus->emit(framework::EventInitialize());
    bus->emit(framework::EventPostInitialize());
    auto loop = core::Singleton<core::EventLoop>::getInstance();
    loop->run();
    bus->emit(framework::EventUnInitialize());
    return exitCode;
  }
  void exit(int code = 0) {
    running = false;
    exitCode = code;
  }
};
}; // namespace aleph::framewrok
