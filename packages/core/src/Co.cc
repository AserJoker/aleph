#ifdef __linux__
#include "core/include/Co.hpp"
#include <cstdint>
#include <ucontext.h>
#include <vector>
using namespace aleph;
using namespace aleph::core;

struct Coroutine {
  uint8_t stack[CO_STACK_SIZE];
  ucontext_t ctx;
  AutoPtr<Task> task;
  bool running;
};

std::vector<Coroutine> coroutines;
static size_t current = 0;

static void onCoroutine(void *parameter) {
  auto ctx = (Coroutine *)parameter;
  ctx->task->run();
  ctx->running = false;
  Co::yield();
}

void Co::setup() {
  coroutines.push_back({});
  auto &ctx = *coroutines.rbegin();
  ctx.running = true;
  getcontext(&ctx.ctx);
}

void Co::cleanup() {}

void Co::yield() {
  if (coroutines.size() > 1) {
    auto old = current;
    current++;
    while (!coroutines[current].running && current < coroutines.size()) {
      coroutines.erase(coroutines.begin() + current);
    }
    if (current >= coroutines.size()) {
      current = 0;
    }
    swapcontext(&coroutines[old].ctx, &coroutines[current].ctx);
  }
}

bool Co::ready() { return coroutines.size() == 1; }

void Co::create(const AutoPtr<Task> &task) {
  coroutines.push_back({});
  auto ctx = &*coroutines.rbegin();
  getcontext(&ctx->ctx);
  ctx->ctx.uc_stack.ss_sp = ctx->stack;
  ctx->ctx.uc_stack.ss_size = CO_STACK_SIZE;
  ctx->running = true;
  ctx->task = task;
  makecontext(&ctx->ctx, (void (*)())onCoroutine, 1, ctx);
}
#endif