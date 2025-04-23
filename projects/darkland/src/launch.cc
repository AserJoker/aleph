#include "core/include/AutoPtr.hpp"
#include "core/include/Object.hpp"
#include "core/include/Singleton.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Terminal.hpp"
#include "system/include/WheelEvent.hpp"
#include <clocale>
#include <cstdio>
#include <cwchar>
#include <string>
#include <thread>
#include <vector>

using namespace aleph;

class Application : public core::Object {
private:
  core::AutoPtr<system::Terminal> _terminal =
      core::Singleton<system::Terminal>::get();

  bool _running = true;

  int _y = 3;

public:
  Application() : _running(true) {
    on(&Application::onInput);
    on(&Application::onWheel);
    on(&Application::onButton);
  }

  ~Application() { _terminal->cleanup(); }

  void onButton(Object *, const system::ButtonEvent &event) {
    if (event.getButton() == 0) {
      _terminal->move(10, 10);
      uint16_t idx = 0;
      for (uint8_t i = 0; i < 16; i++) {
        _terminal->move(10, 10 + i);
        for (uint8_t j = 0; j < 16; j++) {
          _terminal->setColor(idx++);
          printf("%3d ", idx);
        }
      }
      _terminal->move(1, 2);
      _terminal->setColor(core::Color{0x1cfff});
      printf("hello world");
      _terminal->setNormal();
      _terminal->present();
    } else if (event.getButton() == 1) {
      _terminal->setPalette(16, 0xffffff);
      _terminal->present();
    }
  }

  void onWheel(Object *, const system::WheelEvent &event) {}

  void onInput(Object *, const system::InputEvent &event) {
    auto &codes = event.getCodes();
    std::string str = "";
    if (codes[0] == 'q') {
      _running = false;
      return;
    }
  }

  int run() {
    std::string input;
    _terminal->setup();
    _terminal->setMouse(true);
    _terminal->setCursor(false);
    _terminal->clear();
    _terminal->present();
    while (_running) {
      auto size = _terminal->getSize();
      _terminal->move(1, 1);
      _terminal->setNormal();
      printf("%d,%d", size.width, size.height);
      using namespace std::chrono;
      _terminal->pollEvent();
      _terminal->present();
      std::this_thread::sleep_for(10ms);
    }
    _terminal->move(1, 1);
    _terminal->setNormal();
    _terminal->clear();
    _terminal->present();
    _terminal->setCursor(true);
    _terminal->setMouse(false);
    _terminal->cleanup();
    return 0;
  }
};

int main(int argc, char *argv[]) {
  Application app;
  return app.run();
}