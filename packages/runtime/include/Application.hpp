#pragma once
#include "core/include/AutoPtr.hpp"
#include "core/include/EventBase.hpp"
#include "core/include/Object.hpp"
#include "system/include/Terminal.h"
namespace aleph::runtime {
class Application : public core::Object {
private:
  core::AutoPtr<system::Terminal> _terminal;
  bool _running;

public:
  Application();

  ~Application() override;

  int run(int argc, char *argv[]);

  void exit();

  void onEvent(core::Object *emitter, const core::EventBase &event) override;
};
} // namespace aleph::runtime