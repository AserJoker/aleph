#include "core/include/AutoPtr.hpp"
#include "core/include/EventBase.hpp"
#include "core/include/Object.hpp"
#include "core/include/Singleton.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Terminal.hpp"
#include <clocale>
#include <cstdio>
#include <cwchar>
#include <format>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace aleph;
class Application : public core::Object {
private:
  core::AutoPtr<system::Terminal> _terminal;

  bool _running;

public:
  Application() : _running(true) {
    _terminal = core::Singleton<system::Terminal>::get();
    _terminal->addEventListener("input", this);
  }

  void onInput(const system::InputEvent &event) {
    _terminal->clear();
    auto codes = event.getCodes();
    if (codes.size() == 1 && codes[0] == 'q') {
      _running = false;
      return;
    }
    std::string str = "";
    for (size_t index = 0; index < codes.size(); index++) {
      str += std::format("0x{:x}", codes[index]);
      if (index != codes.size() - 1) {
        str += ",";
      }
    }
    _terminal->move(1, 2);
    printf("%s", str.c_str());
    _terminal->present();
  }

  void onEvent(core::Object *_, const core::EventBase &event) {
    if (event.getType() == "input") {
      onInput((const system::InputEvent &)event);
    }
  }

  int run() {
    setlocale(LC_ALL, "");
    std::string input;
    _terminal->setup();
    _terminal->saveCursor();
    _terminal->setMouse(true);
    _terminal->setCursor(false);
    _terminal->clear();
    _terminal->present();
    while (_running) {
      using namespace std::chrono;
      _terminal->pollInput();
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