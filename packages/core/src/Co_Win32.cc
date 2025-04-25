
#include "core/include/AutoPtr.hpp"
#include "core/include/ObjectBase.hpp"
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
};

static std::vector<AutoPtr<Coroutine>> coroutines;

static size_t current = 0;

static void onCoroutine(void *parameter) {
  auto ctx = (Coroutine *)parameter;
  ctx->task->run();
  ctx->running = false;
  Co::yield();
}

void Co::setup() {
  auto coroutine = new Coroutine{};
  coroutine->fiber = ConvertThreadToFiber(NULL);
  coroutine->running = true;
  coroutine->task = nullptr;
  coroutines.push_back(coroutine);
}

void Co::cleanup() { ConvertFiberToThread(); }

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