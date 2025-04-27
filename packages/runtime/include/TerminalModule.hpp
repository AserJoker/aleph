#pragma once
#include "Module.hpp"
#include "QuitEvent.hpp"
#include "core/include/Singleton.hpp"
#include "system/include/Terminal.hpp"
namespace aleph::runtime {
class TerminalModule : public Module {
private:
  bool _running = false;

  core::Singleton<system::Terminal> _terminal;

private:
  void onQuit(Object *, const QuitEvent &);

public:
  void run() override;
};
}; // namespace aleph::runtime