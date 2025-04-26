#include "runtime/include/Application.hpp"
using namespace aleph;
using namespace aleph::runtime;
Application::Application(int argc, char *argv[]) : _running(false) {
  for (int idx = 0; idx < argc; idx++) {
    _args.push_back(argv[idx]);
  }
}
Application::~Application() {}

void Application::exit() { _running = false; }

int Application::run() {
  try {
    _running = true;
    while (_running) {
    }
    return 0;
  } catch (std::exception &e) {
    return -1;
  } catch (...) {
    return -1;
  }
}