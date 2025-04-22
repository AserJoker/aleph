#include "core/include/AutoPtr.hpp"
#include "core/include/EventBase.hpp"
#include "core/include/Object.hpp"
#include "core/include/Singleton.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/KeyboardEvent.hpp"
#include "system/include/Terminal.hpp"
#include "system/include/WheelEvent.hpp"
#include <clocale>
#include <cstdio>
#include <cwchar>
#include <format>
#include <string>
#include <thread>
#include <vector>

using namespace aleph;
class Application : public core::Object {
private:
  core::AutoPtr<system::Terminal> _terminal;

  bool _running;

  int _y = 3;

public:
  Application() : _running(true) {
    _terminal = core::Singleton<system::Terminal>::get();
    _terminal->addEventListener("input", this);
    _terminal->addEventListener("keyboard", this);
    _terminal->addEventListener("button", this);
    _terminal->addEventListener("wheel", this);
  }
  ~Application() { _terminal->cleanup(); }

  void onButton(const system::ButtonEvent &event) {
    _terminal->move(50, 1);
    auto pos = _terminal->getMousePosition();
    printf("                                                    ");
    printf("x: %d,y: %d,button: %d,status: %d", pos.x, pos.y, event.getButton(),
           event.getStatus());
  }

  void onWheel(const system::WheelEvent &event) {
    _terminal->move(50, 2);
    printf("                                                    ");
    printf("wheel:%d", event.getDirection());
  }

  void onInput(const system::InputEvent &event) {
    // _terminal->clear();
    auto codes = event.getCodes();
    std::string str = "";
    if (codes[0] == 'q') {
      _running = false;
      return;
    }
    for (size_t index = 0; index < codes.size(); index++) {
      str += std::format("0x{:x}", codes[index]);
      if (index != codes.size() - 1) {
        str += ",";
      }
    }
    _terminal->move(1, _y++);
    printf("%s", str.c_str());
    _terminal->present();
  }
  void onKeyboard(const system::KeyboardEvent &event) {
    if (event.getKey() == 'q') {
      _running = false;
      return;
    }
    _terminal->move(1, 2);
    printf("0x%llx", event.getKey());
    _terminal->present();
  }

  void onEvent(core::Object *_, const core::EventBase &event) {
    if (event.getType() == "input") {
      onInput((const system::InputEvent &)event);
    } else if (event.getType() == "keyboard") {
      onKeyboard((const system::KeyboardEvent &)event);
    } else if (event.getType() == "button") {
      onButton((const system::ButtonEvent &)event);
    } else if (event.getType() == "wheel") {
      onWheel((const system::WheelEvent &)event);
    }
  }

  int run() {
    // setlocale(LC_ALL, "");
    std::string input;
    _terminal->setup();
    _terminal->saveCursor();
    _terminal->setMouse(true);
    _terminal->setCursor(false);
    _terminal->clear();
    _terminal->present();
    while (_running) {
      using namespace std::chrono;
      _terminal->pollEvent();
      std::this_thread::sleep_for(10ms);
    }
    _terminal->clear();
    _terminal->present();
    _terminal->setCursor(true);
    _terminal->setMouse(false);
    _terminal->move(1, 1);
    _terminal->cleanup();
    return 0;
  }
};
int main(int argc, char *argv[]) {
  Application app;
  return app.run();
}