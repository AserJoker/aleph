
#include <errhandlingapi.h>
#include <format>
#ifdef WIN32
#include "core/include/Color.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Key.hpp"
#include "system/include/ResizeEvent.hpp"
#include "system/include/Terminal.hpp"
#include "system/include/WheelEvent.hpp"
#include <windows.h>

#define IS_ENABLE(val, n) (((val) >> (n)) & 1)
#define IS_DISABLE(val, n) (((val) >> (n) & 1) == 0)

#ifndef STDIN_FILENO
#define STDIN_FILENO stdin
#endif

using namespace aleph;
using namespace aleph::system;
Terminal::Terminal() {}

static DWORD oldInputMode = 0;
static DWORD oldOutputMode = 0;

static HANDLE hInput = 0;
static HANDLE hOutput = 0;

using WindowsException = core::Exception<"WindowsException">;

void Terminal::setup() {
  if (!SetConsoleCP(CP_UTF8)) {
    throw WindowsException(std::format(
        "Failed to set console code page,error code: 0x{:x}", GetLastError()));
  }
  if (!SetConsoleOutputCP(CP_UTF8)) {
    throw WindowsException(
        std::format("Failed to set console output code page,error code: 0x{:x}",
                    GetLastError()));
  }
  hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!hOutput) {
    throw WindowsException(std::format(
        "Failed to get console stdout handle: 0x{:x}", GetLastError()));
  }
  DWORD mode = 0;
  if (!GetConsoleMode(hOutput, &mode)) {
    throw WindowsException(std::format(
        "Failed to get console stdout mode: 0x{:x}", GetLastError()));
  }
  oldInputMode = mode;
  mode |= (ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
  if (!SetConsoleMode(hOutput, mode)) {
    throw WindowsException(std::format(
        "Failed to set console stdout mode: 0x{:x}", GetLastError()));
  }
  mode = 0;
  hInput = GetStdHandle(STD_INPUT_HANDLE);
  if (!hOutput) {
    throw WindowsException(std::format(
        "Failed to get console stdin handle: 0x{:x}", GetLastError()));
  }
  if (!GetConsoleMode(hInput, &mode)) {
    throw WindowsException(std::format(
        "Failed to get console stdin mode: 0x{:x}", GetLastError()));
  }
  oldInputMode = mode;
  if (!SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS)) {
    throw WindowsException(std::format(
        "Failed to set console stdin mode: 0x{:x}", GetLastError()));
  }
  if (!SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT)) {
    throw WindowsException(std::format(
        "Failed to set console stdin mode: 0x{:x}", GetLastError()));
  }
  CONSOLE_SCREEN_BUFFER_INFO csbi = {};
  if (!GetConsoleScreenBufferInfo(hOutput, &csbi)) {
  }
  _size.width = csbi.dwSize.X;
  _size.height = csbi.dwSize.Y;
}
void Terminal::cleanup() {
  SetConsoleMode(hInput, oldInputMode);
  SetConsoleMode(hOutput, oldOutputMode);
}

int64_t Terminal::readInput() {
  static uint32_t mouseStatus = 0;
  INPUT_RECORD event = {};
  DWORD readnum = 0;
  int64_t flag = 0;
  if (!ReadConsoleInput(hInput, &event, 1, &readnum)) {
    return Key::ERR;
  }
  if (event.EventType == KEY_EVENT && event.Event.KeyEvent.bKeyDown) {
    auto keyEvent = event.Event.KeyEvent;
    if (keyEvent.dwControlKeyState & SHIFT_PRESSED) {
      flag |= Key::FLAG_SHIFT;
    }
    if (keyEvent.dwControlKeyState & LEFT_CTRL_PRESSED ||
        keyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED) {
      flag |= Key::FLAG_CTRL;
    }
    if (keyEvent.dwControlKeyState & LEFT_ALT_PRESSED ||
        keyEvent.dwControlKeyState & RIGHT_ALT_PRESSED) {
      flag |= Key::FLAG_META;
    }
    if (keyEvent.wVirtualKeyCode == VK_LEFT) {
      return flag | Key::LEFT;
    }
    if (keyEvent.wVirtualKeyCode == VK_RIGHT) {
      return flag | Key::RIGHT;
    }
    if (keyEvent.wVirtualKeyCode == VK_UP) {
      return flag | Key::UP;
    }
    if (keyEvent.wVirtualKeyCode == VK_DOWN) {
      return flag | Key::DOWN;
    }
    if (keyEvent.wVirtualKeyCode == VK_INSERT) {
      return flag | Key::INSERT;
    }
    if (keyEvent.wVirtualKeyCode == VK_DELETE) {
      return flag | Key::DEL;
    }
    if (keyEvent.wVirtualKeyCode == VK_HOME) {
      return flag | Key::HOME;
    }
    if (keyEvent.wVirtualKeyCode == VK_END) {
      return flag | Key::END;
    }
    if (keyEvent.wVirtualKeyCode == VK_PRIOR) {
      return flag | Key::PAGE_UP;
    }
    if (keyEvent.wVirtualKeyCode == VK_NEXT) {
      return flag | Key::PAGE_DOWN;
    }
    if (keyEvent.wVirtualKeyCode == VK_OEM_2) {
      if (flag & Key::FLAG_SHIFT) {
        return '?';
      }
      return flag | '/';
    }
    if (keyEvent.wVirtualKeyCode >= VK_F1 &&
        keyEvent.wVirtualKeyCode <= VK_F12) {
      return flag | Key::F(keyEvent.wVirtualKeyCode - VK_F1 + 1);
    }
    if (keyEvent.uChar.UnicodeChar == 0) {
      return 0;
    }
    flag &= ~Key::FLAG_SHIFT;
    return flag | keyEvent.uChar.UnicodeChar;
  } else if (event.EventType == WINDOW_BUFFER_SIZE_EVENT) {
    auto sizeEvent = event.Event.WindowBufferSizeEvent;
    if (_size.width != sizeEvent.dwSize.X ||
        _size.height != sizeEvent.dwSize.Y) {
      _size.width = sizeEvent.dwSize.X;
      _size.height = sizeEvent.dwSize.Y;
      emit<ResizeEvent>(_size);
    }
  } else if (event.EventType == MOUSE_EVENT) {
    _mouse.x = event.Event.MouseEvent.dwMousePosition.X;
    _mouse.y = event.Event.MouseEvent.dwMousePosition.Y;

    if (event.Event.MouseEvent.dwEventFlags == 0) {
      bool shift = event.Event.MouseEvent.dwControlKeyState & SHIFT_PRESSED;
      bool control = event.Event.MouseEvent.dwControlKeyState &
                     (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);
      bool alt = event.Event.MouseEvent.dwControlKeyState &
                 (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED);
      for (uint32_t idx = 0; idx < 16; idx++) {
        uint32_t flag = 1 << idx;
        auto button = event.Event.MouseEvent.dwButtonState & flag;
        if (button) {
          if (!(mouseStatus & flag)) {
            mouseStatus |= flag;
            emit(ButtonEvent{idx, true, shift, control, alt});
          }
        } else {
          if (mouseStatus & flag) {
            mouseStatus &= ~flag;
            emit(ButtonEvent{idx, false, shift, control, alt});
          }
        }
      }
    } else if (event.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED) {
      if ((int32_t)event.Event.MouseEvent.dwButtonState > 0) {
        emit(WheelEvent{true});
      } else {
        emit(WheelEvent{false});
      }
    }
  }
  return Key::ERR;
}

void Terminal::parseEvent() {
  auto ch = readInput();
  if (ch == Key::ERR || ch == 0) {
    return;
  }
  if (ch == 0x8) {
    _codes.push_back(0x7f);
  }
  _codes.push_back(ch);
  if (((uint8_t)ch) >> 7 & 1) {
    for (int8_t index = 0; index < 6; index++) {
      if (((((uint8_t)ch) >> ((6 - index)) & 1) & 0xf)) {
        auto next = readInput();
        if (next == Key::ERR) {
          break;
        }
        if (next == 0) {
          continue;
        }
        _codes.push_back(next);
      } else {
        break;
      }
    }
  }
}

void Terminal::pollEvent() {
  DWORD num = 0;
  if (!GetNumberOfConsoleInputEvents(hInput, &num)) {
    return;
  }
  if (!num) {
    return;
  }
  while (num > 0) {
    parseEvent();
    num--;
  }
  if (!_codes.empty()) {
    emit(InputEvent{_codes});
    _codes.clear();
  }
}

void Terminal::present() { fflush(stdout); }

void Terminal::move(int32_t x, int32_t y) {
  fprintf(stderr, "\033[%d;%dH", y, x);
}

void Terminal::clear() {
  fprintf(stderr, "\033[H");
  fprintf(stderr, "\033[J");
}

void Terminal::setCursor(bool visible) {
  fprintf(stderr, "\033[?25%c", visible ? 'h' : 'l');
}

void Terminal::setMouse(bool enable) {}

void Terminal::saveCursor() { fprintf(stderr, "\033[s"); }

void Terminal::loadCursor() { fprintf(stderr, "\033[u"); }

const core::Size &Terminal::getSize() const { return _size; }

const core::Point &Terminal::getMousePosition() const { return _mouse; }

void Terminal::setNormal() { fprintf(stderr, "\033[0m"); }

void Terminal::setBold(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 1 : 22);
}

void Terminal::setHalfBright(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 2 : 22);
}

void Terminal::setItalic(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 3 : 23);
}

void Terminal::setUnderline(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 4 : 24);
}

void Terminal::setBlink(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 5 : 25);
}

void Terminal::setReverseVideo(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 7 : 27);
}
void Terminal::setColor(const TERMINAL_COLOR_16 &color, bool bright) {
  char c = (char)color;
  if (bright) {
    c += 60;
  }
  fprintf(stderr, "\033[%dm", c);
};

void Terminal::setBackgroundColor(const TERMINAL_COLOR_16 &color, bool bright) {
  char c = (char)color;
  c += 10;
  if (bright) {
    c += 60;
  }
  fprintf(stderr, "\033[%dm", c);
};

void Terminal::setColor(uint8_t color) {
  fprintf(stderr, "\033[38;5;%dm", color);
}

void Terminal::setBackgroundColor(uint8_t color) {
  fprintf(stderr, "\033[48;5;%dm", color);
}
void Terminal::setPalette(uint8_t idx, const core::Color &color) {
  fprintf(stderr, "\033]4;%d;rgb:%x/%x/%x\007", idx, color.r, color.g, color.b);
}

void Terminal::setPalette(uint8_t idx, uint32_t color) {
  setPalette(idx, core::Color{color});
}

void Terminal::setColor(const core::Color &color) {
  fprintf(stderr, "\033[38;2;%d;%d;%dm", color.r, color.g, color.b);
}

void Terminal::setBackgroundColor(const core::Color &color) {
  fprintf(stderr, "\033[48;2;%d;%d;%dm", color.r, color.g, color.b);
}

void Terminal::print(const std::string &message) {
  WriteConsole(hOutput, message.c_str(), message.length(), NULL, NULL);
}
#endif