#pragma once
#include "core/include/AutoPtr.hpp"
#include "core/include/EventBase.hpp"
#include "core/include/Object.hpp"
#include "system/include/ButtonPressEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Terminal.hpp"
#include <cstdint>
namespace aleph::runtime {
class Application : public core::Object {
private:
  core::AutoPtr<system::Terminal> _terminal;

  bool _running;

  uint32_t _delay;

protected:
  virtual void onResize();

  virtual void onInput(const system::InputEvent &event);

  virtual void onButtonPress(const system::ButtonPressEvent &event);

public:
  Application();

  ~Application() override;

  int run(int argc, char *argv[]);

  void exit();

  void onEvent(core::Object *emitter, const core::EventBase &event) override;

  void setDelay(uint32_t delay);

  uint32_t getDelay() const;
};
} // namespace aleph::runtime