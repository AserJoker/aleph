#ifdef WIN32
#include "system/include/Terminal.hpp"
#include <windows.h>

using namespace aleph;
using namespace aleph::system;
void Terminal::setup() {
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hOutput, &mode);
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOutput, mode);
}
void Terminal::cleanup() {
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hOutput, &mode);
  mode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOutput, mode);
}

void Terminal::pollEvent() {
  DWORD num = 0;
  INPUT_RECORD event;
  DWORD readnum = 0;
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  GetNumberOfConsoleInputEvents(hInput, &num);
  for (DWORD index = 0; index < num; index++) {
    ReadConsoleInput(hInput, &event, sizeof(event), &readnum);
    if (event.EventType == MOUSE_EVENT) {
    } else if (event.EventType == WINDOW_BUFFER_SIZE_EVENT) {
    } else if (event.EventType == MENU_EVENT) {
    } else if (event.EventType == FOCUS_EVENT) {
    } else if (event.EventType == KEY_EVENT) {
    }
  }
}
#endif