#pragma once
#include "core/include/AutoPtr.hpp"
#include "core/include/Task.hpp"
#ifndef CO_STACK_SIZE
#define CO_STACK_SIZE 4096
#endif
namespace aleph::core {
class Co {
public:
  static void setup();

  static void cleanup();

  static void yield();

  static bool ready();

  static void create(const AutoPtr<Task> &task);
};
} // namespace aleph::core