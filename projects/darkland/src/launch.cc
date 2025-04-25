#include "core/include/AutoPtr.hpp"
#include "core/include/Object.hpp"
#include "core/include/Singleton.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Terminal.hpp"
#include "system/include/WheelEvent.hpp"
#include <cstdio>
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

  void onInput(Object *, const system::InputEvent &event) {}

  int run() {
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
  }
};

int main(int argc, char *argv[]) {
  Application app;
  return app.run();
}