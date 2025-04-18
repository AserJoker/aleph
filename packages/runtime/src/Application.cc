#include "runtime/include/Application.hpp"
#include "core/include/Object.hpp"
#include "system/include/ButtonPressEvent.hpp"
#include "system/include/KeyboardEvent.hpp"
#include "system/include/Terminal.hpp"
#include <clocale>
#include <curses.h>
#include <thread>

using namespace std::chrono;
using namespace aleph;
using namespace aleph::runtime;

int y = 1;

Application::Application() : core::Object(), _running(true), _delay(10) {
  setlocale(LC_ALL, "");
  _terminal = new system::Terminal();
  _terminal->addEventListener("resize", this);
  _terminal->addEventListener("input", this);
  _terminal->addEventListener("button_press", this);
}

Application::~Application() {}

void Application::onResize() {}

void Application::onInput(const system::KeyboardEvent &event) {
  auto key = event.getKey();
  auto size = _terminal->getTerminalSize();
  if (key == L"r") {
    for (uint32_t x = 0; x < size.width; x++) {
      _terminal->print(x, 1, L" ");
    }
    refresh();
  } else if (key == L"escape") {
    exit();
  } else {
    _terminal->printf(0, 1, L"key:%ls", key.c_str());
    refresh();
  }
}

void Application::onButtonPress(const system::ButtonPressEvent &event) {
  _terminal->printf(0, 0, L"button %d click,x=%d,y=%d", event.getButton(),
                    _terminal->getMousePosition().x,
                    _terminal->getMousePosition().y);
}

int Application::run(int argc, char *argv[]) {
  while (_running) {
    _terminal->pollEvent();
    std::this_thread::sleep_for(_delay * 1ms);
  }
  return 0;
}

void Application::onEvent(core::Object *emitter, const core::EventBase &event) {
  if (event.getType() == "resize") {
    onResize();
  } else if (event.getType() == "input") {
    onInput((const system::KeyboardEvent &)event);
  } else if (event.getType() == "button_press") {
    onButtonPress((const system::ButtonPressEvent &)event);
  }
}

void Application::setDelay(uint32_t delay) { _delay = delay; }

uint32_t Application::getDelay() const { return _delay; }

void Application::exit() { _running = false; }