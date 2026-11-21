// examples/hello_triangle/main.cpp

#include <horizon/Application.h>

int main() {
  horizon::core::Application app;

  app.initialize();
  app.run();
  app.shutdown();

  return 0;
}
