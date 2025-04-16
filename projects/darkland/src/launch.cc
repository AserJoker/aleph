#include "runtime/include/Application.hpp"

using namespace aleph;
int main(int argc, char *argv[]) {
  runtime::Application app;
  return app.run(argc, argv);
}