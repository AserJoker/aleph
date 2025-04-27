#pragma once
#include "Brush.hpp"
#include "core/include/AutoPtr.hpp"
#include "core/include/Object.hpp"
#include "runtime/include/System.hpp"
#include "runtime/include/TickEvent.hpp"
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
namespace aleph::video {
class RendererSystem : public runtime::System {
private:
  struct Pixel {
    core::AutoPtr<Brush> brush;
    std::string chr;
    bool operator==(const Pixel &another) const {
      return brush == another.brush && chr == another.chr;
    }
  };

private:
  core::AutoPtr<Brush> _brush;

  std::unordered_map<int32_t, std::map<int32_t, Pixel>> _current;

  core::Singleton<system::Terminal> _terminal;

private:
  void onTick(core::Object *, const runtime::TickEvent &);

public:
  RendererSystem();
};
} // namespace aleph::video