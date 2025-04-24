#pragma once
#include <cstdint>
namespace aleph::system {
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
  constexpr static inline auto F(int n) { return 0x90 + n; }
  constexpr static inline int64_t FLAG_SHIFT = 1 << 16;
  constexpr static inline int64_t FLAG_CTRL = 1 << 17;
  constexpr static inline int64_t FLAG_META = 1 << 18;
  constexpr static inline auto CTRL(char key) { return key - 'a' + 0x1; }
};
} // namespace aleph::system