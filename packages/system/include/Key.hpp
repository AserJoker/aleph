#pragma once
#include <cstdint>
namespace aleph::system {
enum class KEY_RAW {
  ESC = 0x1b,
  BUS1 = 0x4f,
  BUS2 = 0x5b,
  MOUSE = 0x4d,
  BUTTON1_DOWN = 0x20,
  BUTTON2_DOWN = 0x21,
  BUTTON3_DOWN = 0x22,
  BUTTON_UP = 0x23,
  WHEEL_UP = 0x60,
  WHEEL_DOWN = 0x61,
  F_GROUP_2 = 0x31,
  F_GROUP_3 = 0x32,
  GROUP_EOF = 0x7e,
  INSERT = 0x32,
  DEL = 0x33,
  HOME = 0x48,
  END = 0x46,
  PAGE_UP = 0x35,
  PAGE_DOWN = 0x36,
  UP = 0x41,
  DOWN = 0x42,
  RIGHT = 0x43,
  LEFT = 0x44,
  BACKSPACE = 0x7f,
  F1 = 0x50,
  F2 = 0x51,
  F3 = 0x52,
  F4 = 0x53,
  F5 = 0x35,
  F6 = 0x37,
  F7 = 0x38,
  F8 = 0x39,
  F9 = 0x30,
  F10 = 0x31,
  F11 = 0x33,
  F12 = 0x34,
  EOK = 0x7e,
};
class KEY {
public:
  constexpr static inline auto ESC = 0x1b;
  constexpr static inline auto TAB = 0x9;
  constexpr static inline auto BACKSPACE = 0x7f;
  constexpr static inline auto ENTER = 0xd;
  constexpr static inline auto SPACE = 0x20;
  constexpr static inline auto UP = 0x80;
  constexpr static inline auto DOWN = 0x82;
  constexpr static inline auto RIGHT = 0x83;
  constexpr static inline auto LEFT = 0x84;
  constexpr static inline auto INSERT = 0x85;
  constexpr static inline auto DEL = 0x86;
  constexpr static inline auto HOME = 0x87;
  constexpr static inline auto END = 0x88;
  constexpr static inline auto PAGE_UP = 0x89;
  constexpr static inline auto PAGE_DOWN = 0x90;
  constexpr static inline auto F(int n) { return 0x91 + n; }
  constexpr static inline int64_t FLAG_SHIFT = 1 << 16;
  constexpr static inline int64_t FLAG_CTRL = 1 << 17;
  constexpr static inline int64_t FLAG_META = 1 << 18;
};
} // namespace aleph::system