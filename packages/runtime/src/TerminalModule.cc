#include "runtime/include/TerminalModule.hpp"
#include "core/include/Co.hpp"
#include "core/include/Object.hpp"
#include "runtime/include/InitializeEvent.hpp"
#include "runtime/include/QuitEvent.hpp"
#include "runtime/include/TickEvent.hpp"
#include <exception>

using namespace aleph;
using namespace aleph::runtime;
void TerminalModule::run() {
  on(&TerminalModule::onQuit);
  _running = true;
  core::Singleton<core::Co> co;
  try {
    _terminal->setup();
    _terminal->setMouse(true);
    _terminal->setCursor(false);
    _terminal->clear();
    emit<InitializeEvent>();
    while (_running) {
      _terminal->pollEvent();
      emit<TickEvent>();
      co->yield();
    }
    _terminal->move(1, 1);
    _terminal->setNormal();
    _terminal->clear();
    _terminal->setMouse(false);
    _terminal->setCursor(true);
    _terminal->cleanup();
  } catch (...) {
    _terminal->cleanup();
    std::rethrow_exception(std::current_exception());
  }
}

void TerminalModule::onQuit(core::Object *, const QuitEvent &) {
  _running = false;
}