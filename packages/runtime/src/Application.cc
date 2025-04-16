#include "runtime/include/Application.hpp"
#include "core/include/Object.hpp"
#include "system/include/InputEvent.h"
#include "system/include/Terminal.h"
#include <clocale>
#include <thread>

using namespace std::chrono;
using namespace aleph;
using namespace aleph::runtime;

int y = 1;

Application::Application() : core::Object(), _running(true) {
  setlocale(LC_ALL, "");
  _terminal = new system::Terminal();
  _terminal->addEventListener("resize", this);
  _terminal->addEventListener("input", this);
}

Application::~Application() {}

int Application::run(int argc, char *argv[]) {
  while (_running) {
    _terminal->pollEvent();
    std::this_thread::sleep_for(10ms);
  }
  return 0;
}

void Application::onEvent(core::Object *emitter, const core::EventBase &event) {
  if (event.getType() == "resize") {
    _terminal->setColor(0xffffff, 0);
    _terminal->printf(0, 0, "resize");
  } else if (event.getType() == "input") {
    auto &e = (const system::InputEvent &)event;
    auto ch = e.getKey();
    if (ch != -1) {
      if (ch == 'q') {
        exit();
      }
      _terminal->setColor(0xff0000, 0x00ff00);
      _terminal->printf(0, y, "0x%x", ch);
      y++;
    }
  }
}

void Application::exit() { _running = false; }