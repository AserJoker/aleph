#pragma once
#include <cstdint>
#include <format>
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

  constexpr static inline auto KEYNAME(int64_t key) {
    if (key <= 0x7f) {
      return std::string{(char)(key & 0xff)};
    } else if (key == ESC) {
      return std::string{"<ESC>"};
    } else if (key == SPACE) {
      return std::string{"<SPACE>"};
    } else if (key == TAB) {
      return std::string{"<TAB>"};
    } else if (key == ENTER) {
      return std::string{"<ENTER>"};
    } else if (key == UP) {
      return std::string{"<UP>"};
    } else if (key == DOWN) {
      return std::string{"<DOWN>"};
    } else if (key == RIGHT) {
      return std::string{"<RIGHT>"};
    } else if (key == LEFT) {
      return std::string{"<LEFT>"};
    } else if (key == INSERT) {
      return std::string{"<INSERT>"};
    } else if (key == DEL) {
      return std::string{"<DEL>"};
    } else if (key == HOME) {
      return std::string{"<HOME>"};
    } else if (key == END) {
      return std::string{"<END>"};
    } else if (key == PAGE_UP) {
      return std::string{"<PAGE_UP>"};
    } else if (key == PAGE_DOWN) {
      return std::string{"<PAGE_DOWN>"};
    } else if (key >= F(1) && key <= F(12)) {
      return std::format("F{}", key - F(0));
    }
    return std::format("0x{:x}", key);
  }
};
} // namespace aleph::system