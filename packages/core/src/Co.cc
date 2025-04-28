
#ifdef __linux__
#include "core/include/Co.hpp"
#include "core/include/ObjectBase.hpp"
#include "core/include/Singleton.hpp"
#include <cstdint>
#include <exception>
#include <ucontext.h>
#include <vector>

using namespace aleph;
using namespace aleph::core;

struct Coroutine : public ObjectBase {
  uint8_t stack[CO_STACK_SIZE];
  ucontext_t ctx;
  AutoPtr<Task> task;
  bool running;
  std::string error;
};

std::vector<core::AutoPtr<Coroutine>> coroutines;
static size_t current = 0;

static void onCoroutine(void *parameter) {
  auto ctx = (Coroutine *)parameter;
  try {
    ctx->task->run();
  } catch (std::exception &e) {
    coroutines[0]->error = e.what();
  } catch (...) {
    coroutines[0]->error = "Unknown exception";
  }
  ctx->running = false;
  Singleton<Co>::get()->yield();
}

Co::Co() {
  auto ctx = new Coroutine{};
  ctx->running = true;
  getcontext(&ctx->ctx);
  coroutines.push_back(ctx);
}

Co::~Co() {}

void Co::yield() {
  if (coroutines.size() > 1) {
    auto old = current;
    current++;
    while (current < coroutines.size() && !coroutines[current]->running) {
      coroutines.erase(coroutines.begin() + current);
    }
    if (current >= coroutines.size()) {
      current = 0;
    }
    swapcontext(&coroutines[old]->ctx, &coroutines[current]->ctx);
    if (!coroutines[current]->error.empty()) {
      throw CoException{"uncaught exception",
                        std::runtime_error(coroutines[current]->error)};
    }
  }
}

bool Co::ready() { return coroutines.size() == 1; }

void Co::create(const AutoPtr<Task> &task) {
  auto ctx = new Coroutine{};
  getcontext(&ctx->ctx);
  ctx->ctx.uc_stack.ss_sp = ctx->stack;
  ctx->ctx.uc_stack.ss_size = CO_STACK_SIZE;
  ctx->running = true;
  ctx->task = task;
  makecontext(&ctx->ctx, (void (*)())onCoroutine, 1, ctx);
  coroutines.push_back(ctx);
}
#endif