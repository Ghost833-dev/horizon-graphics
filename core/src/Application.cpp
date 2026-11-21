#include <chrono>
#include <iostream>
#include <horizon/Application.h>
#include <horizon/Window.h>
#include <thread>

namespace horizon {
namespace core {

Application::Application() {
  std::cout << "[Horizon Core] Application created" << std::endl;
}

Application::~Application() {
  std::cout << "[Horizon Core] Application destroyed" << std::endl;
  glfwTerminate();
}

void Application::initialize() {
  std::cout << "[Horizon Core] Application initialized" << std::endl;
  m_window = std::make_unique<Window>(800, 600, "Horizon Application");
}

void Application::run() {
  std::cout << "[Horizon Core] Application is running..." << std::endl;

  while (!m_window->shouldClose()) {
    m_window->pollEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    //
  }
}

void Application::shutdown() {
  std::cout << "[Horizon Core] Application shutting down" << std::endl;
  m_window.reset();
}

} // namespace core
} // namespace horizon
