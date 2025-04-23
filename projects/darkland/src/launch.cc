#include "core/include/AutoPtr.hpp"
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

std::wstring UTF8_to_wchar(const char *in) {
  std::wstring out;
  unsigned int codepoint;
  while (*in != 0) {
    unsigned char ch = static_cast<unsigned char>(*in);
    if (ch <= 0x7f)
      codepoint = ch;
    else if (ch <= 0xbf)
      codepoint = (codepoint << 6) | (ch & 0x3f);
    else if (ch <= 0xdf)
      codepoint = ch & 0x1f;
    else if (ch <= 0xef)
      codepoint = ch & 0x0f;
    else
      codepoint = ch & 0x07;
    ++in;
    if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff)) {
      if (sizeof(wchar_t) > 2)
        out.append(1, static_cast<wchar_t>(codepoint));
      else if (codepoint > 0xffff) {
        codepoint -= 0x10000;
        out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
        out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
      } else if (codepoint < 0xd800 || codepoint >= 0xe000)
        out.append(1, static_cast<wchar_t>(codepoint));
    }
  }
  return out;
}

class Application : public core::Object {
private:
  core::AutoPtr<system::Terminal> _terminal =
      core::Singleton<system::Terminal>::get();

  bool _running = true;

  int _y = 3;

public:
  Application() : _running(true) {
    on(&Application::onInput);
    on(&Application::onKeyboard);
    on(&Application::onWheel);
    on(&Application::onButton);
  }

  ~Application() { _terminal->cleanup(); }

  void onButton(Object *, const system::ButtonEvent &event) {
    _terminal->move(10, 1);
    auto &pos = _terminal->getMousePosition();
    printf("                                                    ");
    _terminal->move(10, 1);
    printf("x: %d,y: %d,button: %d,status: %d,shift :%d,control :%d,alt :%d",
           pos.x, pos.y, event.getButton(), event.getStatus(), event.isShift(),
           event.isControl(), event.isAlt());
  }

  void onWheel(Object *, const system::WheelEvent &event) {
    _terminal->move(10, 2);
    printf("                                                    ");
    _terminal->move(10, 2);
    printf("wheel:%d", event.getDirection());
  }

  void onInput(Object *, const system::InputEvent &event) {
    auto &codes = event.getCodes();
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

  void onKeyboard(Object *, const system::KeyboardEvent &event) {
    if (event.getKey() == 'q') {
      _running = false;
      return;
    }
    _terminal->move(1, 2);
    printf("0x%llu", event.getKey());
    _terminal->present();
  }

  int run() {
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