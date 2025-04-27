
#include "core/include/AutoPtr.hpp"
#include "core/include/ObjectBase.hpp"
#include "core/include/Singleton.hpp"
#include <exception>
#include <stdexcept>
#include <string>
#ifdef WIN32
#include "core/include/Co.hpp"

#include <vector>
#include <windows.h>
using namespace aleph;
using namespace aleph::core;

struct Coroutine : public ObjectBase {
  LPVOID fiber;
  bool running;
  core::AutoPtr<Task> task;
  std::string error;
};

static std::vector<AutoPtr<Coroutine>> coroutines;

static size_t current = 0;

static void onCoroutine(void *parameter) {
  auto ctx = (Coroutine *)parameter;
  try {
    ctx->task->run();
    ctx->running = false;
    core::Singleton<Co>::get()->yield();
  } catch (std::exception &e) {
    coroutines[0]->error = e.what();
    ctx->running = false;
    core::Singleton<Co>::get()->yield();
    current = 0;
  } catch (...) {
    coroutines[0]->error = "unknown coroutine error";
    ctx->running = false;
    core::Singleton<Co>::get()->yield();
    current = 0;
  }
}

Co::Co() {
  auto coroutine = new Coroutine{};
  coroutine->fiber = ConvertThreadToFiber(NULL);
  coroutine->running = true;
  coroutine->task = nullptr;
  coroutines.push_back(coroutine);
}

Co::~Co() { ConvertFiberToThread(); }

void Co::yield() {
  if (coroutines.size() > 1) {
    current++;
    while (current < coroutines.size() && !coroutines[current]->running) {
      DeleteFiber(coroutines[current]->fiber);
      coroutines.erase(coroutines.begin() + current);
    }
    if (current >= coroutines.size()) {
      current = 0;
    }
    SwitchToFiber(coroutines[current]->fiber);
    if (!coroutines[current]->error.empty()) {
      throw CoException{"uncaught exception",
                        std::runtime_error(coroutines[current]->error)};
    }
  }
}

bool Co::ready() { return coroutines.size() == 1; }

void Co::create(const AutoPtr<Task> &task) {
  auto coroutine = new Coroutine{};
  coroutine->task = task;
  coroutine->running = true;
  coroutine->fiber = CreateFiber(CO_STACK_SIZE, onCoroutine, coroutine);
  coroutines.push_back(coroutine);
}
#endif