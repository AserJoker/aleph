

#include "system/include/WheelEvent.hpp"
#ifdef WIN32
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Terminal.hpp"
#include <windows.h>

#define IS_ENABLE(val, n) (((val) >> (n)) & 1)
#define IS_DISABLE(val, n) (((val) >> (n) & 1) == 0)

using namespace aleph;
using namespace aleph::system;
Terminal::Terminal() : _mouseStatus(0) {}

void Terminal::setup() {
  SetConsoleCP(CP_UTF8);
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hOutput, &mode);
  mode |= (ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
  SetConsoleMode(hOutput, mode);
  mode = 0;
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hInput, &mode);
  SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS);
  SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hOutput, &csbi);
  _size.width = csbi.dwSize.X;
  _size.height = csbi.dwSize.Y;
}
void Terminal::cleanup() {
  DWORD mode = 0;
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hInput, &mode);
  mode &= ~(ENABLE_VIRTUAL_TERMINAL_INPUT);
  mode |= ENABLE_PROCESSED_INPUT;
  SetConsoleMode(hInput, mode);
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleMode(hOutput, &mode);
  mode &= ~(ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
  SetConsoleMode(hOutput, mode);
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

    if (event.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
    } else if (event.Event.MouseEvent.dwEventFlags == 0) {
      for (uint32_t idx = 0; idx < 16; idx++) {
        uint32_t flag = 1 << idx;
        auto button = event.Event.MouseEvent.dwButtonState & flag;
        if (button) {
          if (!(_mouseStatus & flag)) {
            _mouseStatus |= flag;
            emit(ButtonEvent{idx, true});
          }
        } else {
          if (_mouseStatus & (1 << idx)) {
            _mouseStatus &= ~(1 << idx);
            emit(ButtonEvent{idx, false});
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
  if (ch == -1) {
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
  move(1, 1);
  printf("%d,%d", _size.width, _size.height);
  present();
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

void Terminal::setMouse(bool enable) {
  printf("\033[?1003;%c", enable ? 'h' : 'l');
}

void Terminal::saveCursor() { printf("\033[s"); }

void Terminal::loadCursor() { printf("\033[u"); }

const core::Size &Terminal::getSize() const { return _size; }

const core::Point &Terminal::getMousePosition() const { return _mouse; }

#endif