#ifdef __linux__
#include "system/include/Terminal.hpp"
#include <csignal>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
using namespace aleph;
using namespace aleph::system;
static termios g_origin = {};
void onResize(int) {
  winsize size = {};
  ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
  printf("\033[0;0H");
  printf("%d,%d", size.ws_col, size.ws_row);
  fflush(STDIN_FILENO);
}

void Terminal::setup() {
  signal(SIGWINCH, &onResize);
  tcgetattr(STDIN_FILENO, &g_origin);
  auto current = g_origin;
  current.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  current.c_iflag &= ~(IXON | ICRNL | BRKINT | ISTRIP | INPCK);
  current.c_oflag &= ~(OPOST);
  current.c_cflag |= CS8;
  current.c_cc[VMIN] = 0;
  current.c_cc[VTIME] = 1;
  tcsetattr(STDIN_FILENO, TCSANOW, &current);
}

void Terminal::cleanup() { tcsetattr(STDIN_FILENO, TCSANOW, &g_origin); }

int Terminal::readInput() {
  char result = 0;
  int numbytes = read(STDIN_FILENO, &result, 1);
  if (numbytes == 0) {
    return -1;
  }
  return result;
}

void Terminal::present() { fflush(STDIN_FILENO); }

void Terminal::move(int x, int y) { printf("\033[%d;%dH", y, x); }

void Terminal::clear() { printf("\033[2J"); }

void Terminal::setCursor(bool visible) {
  printf("\033[?25%c", visible ? 'h' : 'l');
}

void Terminal::setMouse(bool enable) {
  printf("\033[?1000%c", enable ? 'h' : 'l');
}
void Terminal::saveCursor() { printf("\033[s"); }

void Terminal::loadCursor() { printf("\033[u"); }
#endif