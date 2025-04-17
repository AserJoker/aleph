#include "runtime/include/Application.hpp"
#include "core/include/Object.hpp"
#include "system/include/ButtonPressEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Terminal.hpp"
#include <clocale>
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

void Application::onInput(const system::InputEvent &event) {
  auto key = event.getKey();
  _terminal->printf(0, 1, "key:0x%x", key);
  if (key == 0x1b) {
    exit();
  }
}

void Application::onButtonPress(const system::ButtonPressEvent &event) {
  _terminal->printf(0, 0, "button %d click,x=%d,y=%d", event.getButton(),
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
    onInput((const system::InputEvent &)event);
  } else if (event.getType() == "button_press") {
    onButtonPress((const system::ButtonPressEvent &)event);
  }
}

void Application::setDelay(uint32_t delay) { _delay = delay; }

uint32_t Application::getDelay() const { return _delay; }

void Application::exit() { _running = false; }