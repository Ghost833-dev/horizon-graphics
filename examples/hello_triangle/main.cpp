// examples/hello_triangle/main.cpp

#include <quantum/core/Application.h>

int main() {
    quantum::core::Application app;

    app.initialize();
    app.printMessage("Hello World from Quantum!");
    app.run();
    app.shutdown();

    return 0;
}