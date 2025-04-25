#pragma once
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

  static void create(void (*entry)());
};
} // namespace aleph::core