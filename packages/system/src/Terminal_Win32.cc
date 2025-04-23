

#include "core/include/Color.hpp"
#ifdef WIN32
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
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
Terminal::Terminal() : _mouseStatus(0) {}

static DWORD oldInputMode = 0;
static DWORD oldOutputMode = 0;

void Terminal::setup() {
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hOutput, &mode);
  oldInputMode = mode;
  mode |= (ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
  SetConsoleMode(hOutput, mode);
  mode = 0;
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hInput, &mode);
  oldInputMode = mode;
  SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS);
  SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);
  CONSOLE_SCREEN_BUFFER_INFOEX csbi = {};
  GetConsoleScreenBufferInfoEx(hOutput, &csbi);
  _size.width = csbi.dwSize.X;
  _size.height = csbi.dwSize.Y;
}
void Terminal::cleanup() {
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  SetConsoleMode(hInput, oldInputMode);
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleMode(hOutput, oldOutputMode);
}

int64_t Terminal::readInput() {
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD event = {};
  DWORD readnum = 0;
  if (!ReadConsoleInput(hInput, &event, 1, &readnum)) {
    return -1;
  }
  if (event.EventType == KEY_EVENT && event.Event.KeyEvent.bKeyDown) {
    return event.Event.KeyEvent.uChar.UnicodeChar;
  } else if (event.EventType == WINDOW_BUFFER_SIZE_EVENT) {
    _size.width = event.Event.WindowBufferSizeEvent.dwSize.X;
    _size.height = event.Event.WindowBufferSizeEvent.dwSize.Y;
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
          if (!(_mouseStatus & flag)) {
            _mouseStatus |= flag;
            emit(ButtonEvent{idx, true, shift, control, alt});
          }
        } else {
          if (_mouseStatus & (flag)) {
            _mouseStatus &= ~(flag);
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
  return -1;
}

void Terminal::parseEvent() {
  auto ch = readInput();
  if (ch == -1 || ch == 0) {
    return;
  }
  _codes.push_back(ch);
  if (((uint8_t)ch) >> 7 & 1) {
    for (int8_t index = 0; index < 6; index++) {
      if (((((uint8_t)ch) >> ((6 - index)) & 1) & 0xf)) {
        auto next = readInput();
        _codes.push_back(next);
      } else {
        break;
      }
    }
  }
}

void Terminal::pollEvent() {
  DWORD num = 0;
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
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

void Terminal::present() { fflush(STDIN_FILENO); }

void Terminal::move(int32_t x, int32_t y) { printf("\033[%d;%dH", y, x); }

void Terminal::clear() {
  printf("\033[H");
  printf("\033[J");
}

void Terminal::setCursor(bool visible) {
  printf("\033[?25%c", visible ? 'h' : 'l');
}

void Terminal::setMouse(bool enable) {}

void Terminal::saveCursor() { printf("\033[s"); }

void Terminal::loadCursor() { printf("\033[u"); }

const core::Size &Terminal::getSize() const { return _size; }

const core::Point &Terminal::getMousePosition() const { return _mouse; }

void Terminal::setNormal() { printf("\033[0m"); }

void Terminal::setBold(bool enable) { printf("\033[%dm", enable ? 1 : 22); }

void Terminal::setHalfBright(bool enable) {
  printf("\033[%dm", enable ? 2 : 22);
}

void Terminal::setItalic(bool enable) { printf("\033[%dm", enable ? 3 : 23); }

void Terminal::setUnderline(bool enable) {
  printf("\033[%dm", enable ? 4 : 24);
}

void Terminal::setBlink(bool enable) { printf("\033[%dm", enable ? 5 : 25); }

void Terminal::setReverseVideo(bool enable) {
  printf("\033[%dm", enable ? 7 : 27);
}
void Terminal::setColor(const TERMINAL_COLOR_16 &color, bool bright) {
  char c = (char)color;
  if (bright) {
    c += 60;
  }
  printf("\033[%dm", c);
};

void Terminal::setBackgroundColor(const TERMINAL_COLOR_16 &color, bool bright) {
  char c = (char)color;
  c += 10;
  if (bright) {
    c += 60;
  }
  printf("\033[%dm", c);
};

void Terminal::setColor(uint8_t color) { printf("\033[38;5;%dm", color); }

void Terminal::setBackgroundColor(uint8_t color) {
  printf("\033[48;5;%dm", color);
}
void Terminal::setPalette(uint8_t idx, const core::Color &color) {
  printf("\033]4;%d;rgb:%x/%x/%x\007", idx, color.r, color.g, color.b);
}

void Terminal::setPalette(uint8_t idx, uint32_t color) {
  setPalette(idx, core::Color{color});
}

void Terminal::setColor(const core::Color &color) {
  printf("\033[38;2;%d;%d;%dm", color.r, color.g, color.b);
}

void Terminal::setBackgroundColor(const core::Color &color) {
  printf("\033[48;2;%d;%d;%dm", color.r, color.g, color.b);
}
#endif