#include "core/include/AutoPtr.hpp"
#include "core/include/Co.hpp"
#include "core/include/Object.hpp"
#include "core/include/Singleton.hpp"
#include "core/include/Task.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Key.hpp"
#include "system/include/Terminal.hpp"
#include "system/include/WheelEvent.hpp"
#include <cstdio>
#include <exception>
#include <iostream>
#include <thread>

using namespace aleph;

class Application : public core::Object {
private:
  core::Singleton<system::Terminal> _terminal;

  bool _running = true;

public:
  Application() : _running(true) {
    on(&Application::onInput);
    on(&Application::onWheel);
    on(&Application::onButton);
  }

  ~Application() { _terminal->cleanup(); }

  void onButton(Object *, const system::ButtonEvent &event) {}

  void onWheel(Object *, const system::WheelEvent &event) {}

  void onInput(Object *, const system::InputEvent &event) {
    auto &codes = event.getCodes();
    if (codes.size() == 1) {
      if (codes[0] == system::Key::ESC) {
        _running = false;
      }
    }
  }

  int run() {
    try {
      _terminal->setup();
      _terminal->setMouse(true);
      _terminal->setCursor(false);
      _terminal->clear();
      _terminal->present();
      while (_running) {
        using namespace std::chrono;
        _terminal->pollEvent();
        _terminal->present();
        std::this_thread::sleep_for(10ms);
      }
      _terminal->move(1, 1);
      _terminal->setNormal();
      _terminal->clear();
      _terminal->present();
      _terminal->setMouse(false);
      _terminal->setCursor(true);
      _terminal->cleanup();
      return 0;
    } catch (std::exception &e) {
      _terminal->cleanup();
      std::cerr << e.what() << std::endl;
      return -1;
    }
  }
};

void fn1() {
  for (int i = 0; i < 10; i++) {
    printf("f1: %d\n", i);
    core::Co::yield();
  }
}

void fn2() {
  for (int i = 0; i < 5; i++) {
    printf("f2: %d\n", i);
    core::Co::yield();
  }
}

void fn3() {
  for (int i = 0; i < 10; i++) {
    printf("f3: %d\n", i);
    core::Co::yield();
  }
}

int main(int argc, char *argv[]) {
  // Application app;
  // return app.run();
  core::Co::setup();
  core::Co::create(new core::FunctionalTask(fn1));
  core::Co::create(new core::FunctionalTask(fn2));
  core::Co::create(new core::FunctionalTask(fn3));
  while (!core::Co::ready()) {
    core::Co::yield();
  }
  core::Co::cleanup();
  return 0;
}