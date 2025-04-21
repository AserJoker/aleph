
#ifdef WIN32
#include "system/include/InputEvent.hpp"
#include "system/include/Terminal.hpp"
#include <windows.h>

using namespace aleph;
using namespace aleph::system;
void Terminal::setup() {
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hOutput, &mode);
  mode |= (ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
  SetConsoleMode(hOutput, mode);
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(hInput, &mode);
  mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
  mode &= ~(ENABLE_PROCESSED_INPUT);
  SetConsoleMode(hInput, mode);

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

int Terminal::readInput() {
  DWORD num = 0;
  INPUT_RECORD event = {};
  DWORD readnum = 0;
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  if (GetNumberOfConsoleInputEvents(hInput, &num)) {
    std::vector<int> codes;
    while (num > 0) {
      ReadConsoleInput(hInput, &event, 1, &readnum);
      if (event.EventType == KEY_EVENT && event.Event.KeyEvent.bKeyDown) {
        codes.push_back(event.Event.KeyEvent.uChar.AsciiChar);
      } else if (event.EventType == WINDOW_BUFFER_SIZE_EVENT) {
        _size.width = event.Event.WindowBufferSizeEvent.dwSize.X;
        _size.height = event.Event.WindowBufferSizeEvent.dwSize.Y;
      }
      num--;
    }
    if (!codes.empty()) {
      emit(InputEvent{codes});
      return TRUE;
    }
  }
  move(1, 1);
  printf("%d,%d", _size.width, _size.height);
  return FALSE;
}

void Terminal::present() { fflush(STDIN_FILENO); }

void Terminal::move(int x, int y) { printf("\033[%d;%dH", y, x); }

void Terminal::clear() {
  printf("\033[H");
  printf("\033[J");
}

void Terminal::setCursor(bool visible) {
  printf("\033[?25%c", visible ? 'h' : 'l');
}

void Terminal::setMouse(bool enable) {
  printf("\033[?1000%c", enable ? 'h' : 'l');
}
void Terminal::saveCursor() { printf("\033[s"); }

void Terminal::loadCursor() { printf("\033[u"); }
#endif