#include "core/include/Co.hpp"
#include <vector>
#include <windows.h>
using namespace aleph;
using namespace aleph::core;

struct Coroutine {
  LPVOID fiber;
  bool running;
  void (*entry)();
};

static std::vector<Coroutine> coroutines;

static size_t current = 0;

static void onCoroutine(void *parameter) {
  Coroutine *ctx = (Coroutine *)parameter;
  ctx->entry();
  ctx->running = false;
  Co::yield();
}

void Co::setup() { coroutines.push_back({ConvertThreadToFiber(NULL), true}); }

void Co::cleanup() { ConvertFiberToThread(); }

void Co::yield() {
  if (coroutines.size() > 1) {
    current++;
    while (!coroutines[current].running && current < coroutines.size()) {
      DeleteFiber(coroutines[current].fiber);
    }
    if (current >= coroutines.size()) {
      current = 0;
    }
    SwitchToFiber(coroutines[current].fiber);
  }
}

bool Co::ready() { return coroutines.size() == 1; }

void Co::create(void (*entry)()) {
  coroutines.push_back({
      nullptr,
      true,
      entry,
  });
  auto ctx = &*coroutines.rbegin();
  ctx->fiber = CreateFiber(CO_STACK_SIZE, onCoroutine, (void *)ctx);
}
