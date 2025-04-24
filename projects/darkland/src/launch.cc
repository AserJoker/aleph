#include "core/include/AutoPtr.hpp"
#include "core/include/Color.hpp"
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
          _terminal->setBackgroundColor(core::Color{(uint8_t)idx, 0, 0});
          printf("%3d ", idx);
          idx++;
        }
      }
      _terminal->move(1, 3);
      _terminal->setNormal();
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
    if (codes[0] == 'r') {
      _terminal->clear();
      return;
    }
    for (auto idx = 0; idx < codes.size(); idx++) {
      auto ch = (char)(codes[idx] & 0xff);
      if (ch == system::KEY::ESC) {
        str += "<ESC>";
      } else if (ch == system::KEY::TAB) {
        str += "<TAB>";
      } else if (ch == system::KEY::BACKSPACE) {
        str += "<BACKSPACE>";
      } else if (ch == system::KEY::SPACE) {
        str += "<SPACE>";
      } else if (ch >= 0x20 && ch < 0x7f) {
        str += ch;
      } else {
        str += std::format("0x{:x}", ch);
      }
      // str += std::format("0x{:x}", codes[idx] & 0xff);
      if (codes.size() == 1) {
        str += std::format(", shift: {}, ctrl: {}, meta: {}",
                           (codes[idx] & system::KEY::FLAG_SHIFT) != 0,
                           (codes[idx] & system::KEY::FLAG_CTRL) != 0,
                           (codes[idx] & system::KEY::FLAG_META) != 0);
      }
      // if (idx != codes.size() - 1) {
      //   str += ",";
      // }
    }
    _terminal->move(1, 4);
    printf("                                                 ");
    _terminal->move(1, 4);
    printf("%s", str.c_str());
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
      printf("%d,%d", size.width, size.height);
      _terminal->move(1, 2);
      _terminal->setNormal();
      auto &mouse = _terminal->getMousePosition();
      printf("%d,%d", mouse.x, mouse.y);
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