#include "core/include/AutoPtr.hpp"
#include "core/include/Object.hpp"
#include "core/include/Singleton.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Key.hpp"
#include "system/include/Terminal.hpp"
#include "system/include/WheelEvent.hpp"
#include <cstdio>
#include <format>
#include <string>
#include <thread>
#include <vector>

using namespace aleph;

class Application : public core::Object {
private:
  core::AutoPtr<system::Terminal> _terminal =
      core::Singleton<system::Terminal>::get();

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
    std::string str = "";
    if (codes[0] == 'q') {
      _running = false;
      return;
    }
    if (codes[0] == 'r') {
      _terminal->clear();
      return;
    }
    if (codes.size() == 1) {
      auto ch = (uint8_t)(codes[0] & 0xff);
      bool ctrl = false;
      if (ch < 0x20) {
        str += system::Key::name(ch + 0x40);
        ctrl = true;
      } else {
        str += system::Key::name(ch);
      }
      if (codes.size() == 1) {
        str += std::format(", shift: {}, ctrl: {}, meta: {}",
                           (codes[0] & system::Key::FLAG_SHIFT) != 0,
                           (codes[0] & system::Key::FLAG_CTRL) != 0 || ctrl,
                           (codes[0] & system::Key::FLAG_META) != 0);
      }
    } else {
      for (auto idx = 0; idx < codes.size(); idx++) {
        str.push_back(codes[idx]);
      }
    }
    _terminal->move(1, 3);
    _terminal->print(str);
  }

  int run() {
    std::string input;
    _terminal->setup();
    _terminal->setMouse(true);
    _terminal->setCursor(false);
    _terminal->clear();
    _terminal->present();
    while (_running) {
      auto &size = _terminal->getSize();
      _terminal->move(1, 1);
      _terminal->setNormal();
      _terminal->print(std::format("{},{}", size.width, size.height));
      _terminal->move(1, 2);
      _terminal->setNormal();
      auto &mouse = _terminal->getMousePosition();
      _terminal->print(std::format("{},{}", mouse.x, mouse.y));
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