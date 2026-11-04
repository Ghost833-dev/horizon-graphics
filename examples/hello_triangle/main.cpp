// examples/hello_triangle/main.cpp

#include <quantum/Application.h>

int main() {
  quantum::core::Application app;

  app.initialize();
  app.run();
  app.shutdown();

  return 0;
}
