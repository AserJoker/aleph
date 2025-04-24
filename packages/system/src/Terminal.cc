#include "system/include/Terminal.hpp"
#include "core/include/Size.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Key.hpp"
#include "system/include/ResizeEvent.hpp"
#include "system/include/WheelEvent.hpp"

#ifdef __linux__
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
    _codes.push_back(KEY::SPACE | KEY::FLAG_CTRL);
    return;
  }
  if (ch == 0x1f) {
    _codes.push_back('/' | KEY::FLAG_CTRL);
    return;
  }
  if (ch == KEY::ESC) {
    while (true) {
      if (!_codes.empty() && (ch == -1 || ch == KEY::ESC)) {
        if (_codes[0] == KEY::ESC) {
          if (_codes.size() == 1) {
            emit(InputEvent{_codes});
            _codes.clear();
          } else if (_codes.size() == 2) {
            emit(InputEvent{{_codes[1] | KEY::FLAG_META}}); // ^{x}
            _codes.clear();
          } else {
            if (_codes[1] == '[') {
              if (_codes.size() == 3) {
                if (_codes[2] == 'A') {
                  emit({InputEvent{{KEY::UP}}}); // ESC[A
                  _codes.clear();
                }
                if (_codes[2] == 'B') {
                  emit({InputEvent{{KEY::DOWN}}});
                  _codes.clear();
                }
                if (_codes[2] == 'C') {
                  emit({InputEvent{{KEY::RIGHT}}});
                  _codes.clear();
                }
                if (_codes[2] == 'D') {
                  emit({InputEvent{{KEY::LEFT}}});
                  _codes.clear();
                }
                if (_codes[2] == 'F') {
                  emit({InputEvent{{KEY::END}}});
                  _codes.clear();
                }
                if (_codes[2] == 'H') {
                  emit({InputEvent{{KEY::HOME}}});
                  _codes.clear();
                }
                if (_codes[2] == 'Z') {
                  emit({InputEvent{{KEY::TAB | KEY::FLAG_SHIFT}}});
                  _codes.clear();
                }
              }
              if (_codes.size() == 4) {
                if (_codes[3] == '~') {
                  if (_codes[2] == '2') {
                    emit(InputEvent{{KEY::INSERT}}); // ESC[2~
                    _codes.clear();
                  }
                  if (_codes[2] == '3') {
                    emit(InputEvent{{KEY::DEL}});
                    _codes.clear();
                  }
                  if (_codes[2] == '5') {
                    emit(InputEvent{{KEY::PAGE_UP}});
                    _codes.clear();
                  }
                  if (_codes[2] == '6') {
                    emit(InputEvent{{KEY::PAGE_DOWN}});
                    _codes.clear();
                  }
                }
              }
              if (_codes.size() == 5) {
                if (_codes[4] == '~') {
                  if (_codes[2] == '1' && _codes[3] == '5') {
                    emit(InputEvent({KEY::F(5)}));
                    _codes.clear();
                  }
                  if (_codes[2] == '1' && _codes[3] == '7') {
                    emit(InputEvent({KEY::F(6)}));
                    _codes.clear();
                  }
                  if (_codes[2] == '1' && _codes[3] == '8') {
                    emit(InputEvent({KEY::F(7)}));
                    _codes.clear();
                  }
                  if (_codes[2] == '1' && _codes[3] == '9') {
                    emit(InputEvent({KEY::F(8)}));
                    _codes.clear();
                  }
                  if (_codes[2] == '2' && _codes[3] == '0') {
                    emit(InputEvent({KEY::F(9)}));
                    _codes.clear();
                  }
                  if (_codes[2] == '2' && _codes[3] == '1') {
                    emit(InputEvent({KEY::F(10)}));
                    _codes.clear();
                  }
                  if (_codes[2] == '2' && _codes[3] == '3') {
                    emit(InputEvent({KEY::F(11)}));
                    _codes.clear();
                  }
                  if (_codes[2] == '2' && _codes[3] == '4') {
                    emit(InputEvent({KEY::F(12)}));
                    _codes.clear();
                  }
                }
              }
              if (_codes.size() == 6) {
                if (_codes[2] == '1' && _codes[3] == ';') {
                  int64_t flag = 0;
                  if (_codes[4] == '2') {
                    flag = KEY::FLAG_SHIFT;
                  }
                  if (_codes[4] == '3') {
                    flag = KEY::FLAG_CTRL;
                  }
                  if (_codes[4] == '5') {
                    flag = KEY::FLAG_META;
                  }
                  if (_codes[5] == 'A') {
                    emit(InputEvent{{KEY::UP | flag}}); // ESC[1;2A
                    _codes.clear();
                  }
                  if (_codes[5] == 'B') {
                    emit(InputEvent{{KEY::DOWN | flag}});
                    _codes.clear();
                  }
                  if (_codes[5] == 'C') {
                    emit(InputEvent{{KEY::RIGHT | flag}});
                    _codes.clear();
                  }
                  if (_codes[5] == 'D') {
                    emit(InputEvent{{KEY::LEFT | flag}});
                    _codes.clear();
                  }
                  if (_codes[5] == 'F') {
                    emit(InputEvent{{KEY::END | flag}});
                    _codes.clear();
                  }
                  if (_codes[5] == 'H') {
                    emit(InputEvent{{KEY::HOME | flag}});
                    _codes.clear();
                  }
                  if (_codes[5] >= 'P' && _codes[5] <= 'S') {
                    emit(InputEvent{{KEY::F(_codes[5] - 'P' + 1) | flag}});
                    _codes.clear();
                  }
                }
                if (_codes[5] == '~' && _codes[3] == ';') {
                  int64_t flag = 0;
                  if (_codes[4] == '2') {
                    flag = KEY::FLAG_SHIFT;
                  }
                  if (_codes[4] == '3') {
                    flag = KEY::FLAG_CTRL;
                  }
                  if (_codes[4] == '5') {
                    flag = KEY::FLAG_META;
                  }
                  if (_codes[2] == '2') {
                    emit(InputEvent{{KEY::INSERT | flag}});
                    _codes.clear();
                  }
                  if (_codes[2] == '3') {
                    emit(InputEvent{{KEY::DEL | flag}});
                    _codes.clear();
                  }
                  if (_codes[2] == '5') {
                    emit(InputEvent{{KEY::PAGE_UP | flag}});
                    _codes.clear();
                  }
                  if (_codes[2] == '6') {
                    emit(InputEvent{{KEY::PAGE_DOWN | flag}});
                    _codes.clear();
                  }
                }
              }
              if (_codes.size() == 7) {
                int64_t flag = 0;
                if (_codes[6] == '~' && _codes[4] == ';') {
                  if (_codes[5] == '2') {
                    flag = KEY::FLAG_SHIFT;
                  }
                  if (_codes[6] == '3') {
                    flag = KEY::FLAG_CTRL;
                  }
                  if (_codes[6] == '4') {
                    flag = KEY::FLAG_META;
                  }
                  if (_codes[3] == '1' && _codes[4] == '5') {
                    emit(InputEvent({KEY::F(5) | flag}));
                    _codes.clear();
                  }
                  if (_codes[3] == '1' && _codes[4] == '7') {
                    emit(InputEvent({KEY::F(6) | flag}));
                    _codes.clear();
                  }
                  if (_codes[3] == '1' && _codes[4] == '8') {
                    emit(InputEvent({KEY::F(7) | flag}));
                    _codes.clear();
                  }
                  if (_codes[3] == '1' && _codes[4] == '9') {
                    emit(InputEvent({KEY::F(8) | flag}));
                    _codes.clear();
                  }
                  if (_codes[3] == '2' && _codes[4] == '0') {
                    emit(InputEvent({KEY::F(9) | flag}));
                    _codes.clear();
                  }
                  if (_codes[3] == '2' && _codes[4] == '1') {
                    emit(InputEvent({KEY::F(10) | flag}));
                    _codes.clear();
                  }
                  if (_codes[3] == '2' && _codes[4] == '3') {
                    emit(InputEvent({KEY::F(11) | flag}));
                    _codes.clear();
                  }
                  if (_codes[3] == '2' && _codes[4] == '4') {
                    emit(InputEvent({KEY::F(12) | flag}));
                    _codes.clear();
                  }
                }
              }
            }
            if (_codes.size() == 3 && _codes[1] == 'O' && _codes[2] >= 'P' &&
                _codes[2] <= 'S') {
              emit(InputEvent{{KEY::F(_codes[2] - 'P' + 1)}});
              _codes.clear();
            }
            if (_codes[2] == '<') {
              uint32_t btn = 0;
              int32_t x = 0;
              int32_t y = 0;
              _codes.push_back(0);
              std::string str(_codes.begin(), _codes.end());
              if (_codes[_codes.size() - 2] == 'm') {
                sscanf(str.c_str(), "\033[<%d;%d;%dm", &btn, &x, &y);
                if (btn == 35) {
                  _mouse.x = x;
                  _mouse.y = y;
                } else {
                  emit(ButtonEvent{btn, false});
                }
                _codes.clear();
              }
              if (_codes[_codes.size() - 2] == 'M') {
                sscanf(str.c_str(), "\033[<%d;%d;%dM", &btn, &x, &y);
                if (btn == 64) {
                  emit(WheelEvent{true});
                } else if (btn == 65) {
                  emit(WheelEvent{true});
                } else {
                  emit(ButtonEvent{btn, true});
                }
                _codes.clear();
              }
            }
          }
        }
        if (!_codes.empty()) {
          emit(InputEvent{_codes});
          _codes.clear();
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