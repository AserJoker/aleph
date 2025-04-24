#ifdef __linux__
#include "system/include/Terminal.hpp"
#include "core/include/Size.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Key.hpp"
#include "system/include/ResizeEvent.hpp"
#include "system/include/WheelEvent.hpp"
#include <csignal>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
using namespace aleph;
using namespace aleph::system;
static termios g_origin = {};

Terminal::Terminal() {}

static winsize g_terminalSize = {};

void onResize(int) { ioctl(STDOUT_FILENO, TIOCGWINSZ, &g_terminalSize); }

void Terminal::setup() {
  signal(SIGWINCH, &onResize);
  tcgetattr(STDIN_FILENO, &g_origin);
  auto current = g_origin;
  current.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  current.c_iflag &= ~(IXON | ICRNL | BRKINT | ISTRIP | INPCK);
  current.c_oflag &= ~(OPOST);
  current.c_cflag |= CS8;
  current.c_cc[VMIN] = 0;
  current.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &current);
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &g_terminalSize);
}

void Terminal::cleanup() {
  while (!feof(stdin)) {
    fgetc(stdin);
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &g_origin);
}

int64_t Terminal::readInput() {
  fflush(STDIN_FILENO);
  uint8_t ch = 0;
  if (!read(STDIN_FILENO, &ch, 1)) {
    return -1;
  }
  return ch;
}
void Terminal::parseEvent() {
  auto ch = readInput();
  if (ch == -1) {
    return;
  }
  if (ch == 0) {
    _codes.push_back(Key::SPACE | Key::FLAG_CTRL);
    return;
  }
  if (ch == 0x1f) {
    _codes.push_back('/' | Key::FLAG_CTRL);
    return;
  }
  if (ch == Key::ESC) {
    while (true) {
      if (ch == -1 || ch == Key::ESC) {
        if (!_codes.empty()) {
          if (_codes[0] == Key::ESC) {
            if (_codes.size() == 1) {
              emit(InputEvent{_codes});
              _codes.clear();
            } else if (_codes.size() == 2) {
              emit(InputEvent{{_codes[1] | Key::FLAG_META}}); // ^{x}
              _codes.clear();
            } else {
              if (_codes[1] == '[') {
                if (_codes.size() == 3) {
                  if (_codes[2] == 'A') {
                    emit({InputEvent{{Key::UP}}}); // ESC[A
                    _codes.clear();
                  }
                  if (_codes[2] == 'B') {
                    emit({InputEvent{{Key::DOWN}}});
                    _codes.clear();
                  }
                  if (_codes[2] == 'C') {
                    emit({InputEvent{{Key::RIGHT}}});
                    _codes.clear();
                  }
                  if (_codes[2] == 'D') {
                    emit({InputEvent{{Key::LEFT}}});
                    _codes.clear();
                  }
                  if (_codes[2] == 'F') {
                    emit({InputEvent{{Key::END}}});
                    _codes.clear();
                  }
                  if (_codes[2] == 'H') {
                    emit({InputEvent{{Key::HOME}}});
                    _codes.clear();
                  }
                  if (_codes[2] == 'Z') {
                    emit({InputEvent{{Key::TAB | Key::FLAG_SHIFT}}});
                    _codes.clear();
                  }
                }
                if (_codes.size() == 4) {
                  if (_codes[3] == '~') {
                    if (_codes[2] == '2') {
                      emit(InputEvent{{Key::INSERT}}); // ESC[2~
                      _codes.clear();
                    }
                    if (_codes[2] == '3') {
                      emit(InputEvent{{Key::DEL}});
                      _codes.clear();
                    }
                    if (_codes[2] == '5') {
                      emit(InputEvent{{Key::PAGE_UP}});
                      _codes.clear();
                    }
                    if (_codes[2] == '6') {
                      emit(InputEvent{{Key::PAGE_DOWN}});
                      _codes.clear();
                    }
                  }
                }
                if (_codes.size() == 5) {
                  if (_codes[4] == '~') {
                    if (_codes[2] == '1' && _codes[3] == '5') {
                      emit(InputEvent({Key::F(5)}));
                      _codes.clear();
                    }
                    if (_codes[2] == '1' && _codes[3] == '7') {
                      emit(InputEvent({Key::F(6)}));
                      _codes.clear();
                    }
                    if (_codes[2] == '1' && _codes[3] == '8') {
                      emit(InputEvent({Key::F(7)}));
                      _codes.clear();
                    }
                    if (_codes[2] == '1' && _codes[3] == '9') {
                      emit(InputEvent({Key::F(8)}));
                      _codes.clear();
                    }
                    if (_codes[2] == '2' && _codes[3] == '0') {
                      emit(InputEvent({Key::F(9)}));
                      _codes.clear();
                    }
                    if (_codes[2] == '2' && _codes[3] == '1') {
                      emit(InputEvent({Key::F(10)}));
                      _codes.clear();
                    }
                    if (_codes[2] == '2' && _codes[3] == '3') {
                      emit(InputEvent({Key::F(11)}));
                      _codes.clear();
                    }
                    if (_codes[2] == '2' && _codes[3] == '4') {
                      emit(InputEvent({Key::F(12)}));
                      _codes.clear();
                    }
                  }
                }
                if (_codes.size() == 6) {
                  if (_codes[2] == '1' && _codes[3] == ';') {
                    int64_t flag = 0;
                    if (_codes[4] == '2') {
                      flag = Key::FLAG_SHIFT;
                    }
                    if (_codes[4] == '3') {
                      flag = Key::FLAG_CTRL;
                    }
                    if (_codes[4] == '5') {
                      flag = Key::FLAG_META;
                    }
                    if (_codes[5] == 'A') {
                      emit(InputEvent{{Key::UP | flag}}); // ESC[1;2A
                      _codes.clear();
                    }
                    if (_codes[5] == 'B') {
                      emit(InputEvent{{Key::DOWN | flag}});
                      _codes.clear();
                    }
                    if (_codes[5] == 'C') {
                      emit(InputEvent{{Key::RIGHT | flag}});
                      _codes.clear();
                    }
                    if (_codes[5] == 'D') {
                      emit(InputEvent{{Key::LEFT | flag}});
                      _codes.clear();
                    }
                    if (_codes[5] == 'F') {
                      emit(InputEvent{{Key::END | flag}});
                      _codes.clear();
                    }
                    if (_codes[5] == 'H') {
                      emit(InputEvent{{Key::HOME | flag}});
                      _codes.clear();
                    }
                    if (_codes[5] >= 'P' && _codes[5] <= 'S') {
                      emit(InputEvent{{Key::F(_codes[5] - 'P' + 1) | flag}});
                      _codes.clear();
                    }
                  }
                  if (_codes[5] == '~' && _codes[3] == ';') {
                    int64_t flag = 0;
                    if (_codes[4] == '2') {
                      flag = Key::FLAG_SHIFT;
                    }
                    if (_codes[4] == '3') {
                      flag = Key::FLAG_CTRL;
                    }
                    if (_codes[4] == '5') {
                      flag = Key::FLAG_META;
                    }
                    if (_codes[2] == '2') {
                      emit(InputEvent{{Key::INSERT | flag}});
                      _codes.clear();
                    }
                    if (_codes[2] == '3') {
                      emit(InputEvent{{Key::DEL | flag}});
                      _codes.clear();
                    }
                    if (_codes[2] == '5') {
                      emit(InputEvent{{Key::PAGE_UP | flag}});
                      _codes.clear();
                    }
                    if (_codes[2] == '6') {
                      emit(InputEvent{{Key::PAGE_DOWN | flag}});
                      _codes.clear();
                    }
                  }
                }
                if (_codes.size() == 7) {
                  int64_t flag = 0;
                  if (_codes[6] == '~' && _codes[4] == ';') {
                    if (_codes[5] == '2') {
                      flag = Key::FLAG_SHIFT;
                    }
                    if (_codes[6] == '3') {
                      flag = Key::FLAG_CTRL;
                    }
                    if (_codes[6] == '4') {
                      flag = Key::FLAG_META;
                    }
                    if (_codes[3] == '1' && _codes[4] == '5') {
                      emit(InputEvent({Key::F(5) | flag}));
                      _codes.clear();
                    }
                    if (_codes[3] == '1' && _codes[4] == '7') {
                      emit(InputEvent({Key::F(6) | flag}));
                      _codes.clear();
                    }
                    if (_codes[3] == '1' && _codes[4] == '8') {
                      emit(InputEvent({Key::F(7) | flag}));
                      _codes.clear();
                    }
                    if (_codes[3] == '1' && _codes[4] == '9') {
                      emit(InputEvent({Key::F(8) | flag}));
                      _codes.clear();
                    }
                    if (_codes[3] == '2' && _codes[4] == '0') {
                      emit(InputEvent({Key::F(9) | flag}));
                      _codes.clear();
                    }
                    if (_codes[3] == '2' && _codes[4] == '1') {
                      emit(InputEvent({Key::F(10) | flag}));
                      _codes.clear();
                    }
                    if (_codes[3] == '2' && _codes[4] == '3') {
                      emit(InputEvent({Key::F(11) | flag}));
                      _codes.clear();
                    }
                    if (_codes[3] == '2' && _codes[4] == '4') {
                      emit(InputEvent({Key::F(12) | flag}));
                      _codes.clear();
                    }
                  }
                }
              }
              if (_codes.size() == 3 && _codes[1] == 'O' && _codes[2] >= 'P' &&
                  _codes[2] <= 'S') {
                emit(InputEvent{{Key::F(_codes[2] - 'P' + 1)}});
                _codes.clear();
              }
              if (_codes[2] == '<') {
                uint32_t btn = 0;
                int32_t x = 0;
                int32_t y = 0;
                std::string str;
                while (!_codes.empty()) {
                  auto ch = *_codes.begin();
                  _codes.erase(_codes.begin());
                  str.push_back(ch);
                  if (ch == 'm' || ch == 'M') {
                    break;
                  }
                }
                if (str[str.length() - 1] == 'm') {
                  sscanf(str.c_str(), "\033[<%d;%d;%dm", &btn, &x, &y);
                  if (btn == 35 || btn == 51 || btn == 43) {
                    _mouse.x = x;
                    _mouse.y = y;
                  } else {
                    if (btn >= 0 && btn < 4) {
                      emit(ButtonEvent{btn, false});
                    } else if (btn >= 4 && btn < 7) {
                      emit(ButtonEvent{btn - 4, false, true, false, false});
                    } else if (btn >= 8 && btn < 11) {
                      emit(ButtonEvent{btn - 8, false, false, false, true});
                    } else if (btn >= 16 && btn < 19) {
                      emit(ButtonEvent{btn - 16, false, false, true, false});
                    }
                  }
                }
                if (str[str.length() - 1] == 'M') {
                  sscanf(str.c_str(), "\033[<%d;%d;%dM", &btn, &x, &y);
                  if (btn == 35 || btn == 51 || btn == 43) {
                    _mouse.x = x;
                    _mouse.y = y;
                  } else if (btn == 64 || btn == 68 || btn == 80 || btn == 72) {
                    emit(WheelEvent{true});
                  } else if (btn == 65 || btn == 69 || btn == 81 || btn == 73) {
                    emit(WheelEvent{true});
                  } else {
                    if (btn >= 0 && btn < 4) {
                      emit(ButtonEvent{btn, true});
                    } else if (btn >= 4 && btn < 7) {
                      emit(ButtonEvent{btn - 4, true, true, false, false});
                    } else if (btn >= 8 && btn < 11) {
                      emit(ButtonEvent{btn - 8, true, false, false, true});
                    } else if (btn >= 16 && btn < 19) {
                      emit(ButtonEvent{btn - 16, true, false, true, false});
                    }
                  }
                }
              }
            }
          } else {
            emit(InputEvent{_codes});
            _codes.clear();
          }
        }
      }
      if (ch == -1) {
        break;
      }
      _codes.push_back(ch);
      ch = readInput();
    }
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
  if (_size.height != g_terminalSize.ws_row ||
      _size.width != g_terminalSize.ws_col) {
    _size.height = g_terminalSize.ws_row;
    _size.width = g_terminalSize.ws_col;
    emit<ResizeEvent>(_size);
  }
  parseEvent();
  if (!_codes.empty()) {
    emit(InputEvent{_codes});
    _codes.clear();
  }
}

void Terminal::present() { fflush(stdout); }

void Terminal::move(int x, int y) { printf("\033[%d;%dH", y, x); }

void Terminal::clear() { write(STDOUT_FILENO, "\033[2J", 4); }

void Terminal::setCursor(bool visible) {
  printf("\033[?25%c", visible ? 'h' : 'l');
}

void Terminal::setMouse(bool enable) {
  printf("\033[?1003;1006%c", enable ? 'h' : 'l');
}
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