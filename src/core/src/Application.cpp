// src/core/src/Application.cpp

#include <chrono>
#include <iostream>
#include <quantum/core/Application.h>
#include <quantum/core/Window.h>
#include <thread>

namespace quantum {
namespace core {

Application::Application() {
  std::cout << "[Quantum Core] Application created" << std::endl;
}

Application::~Application() {
  std::cout << "[Quantum Core] Application destroyed" << std::endl;
}

void Application::initialize() {
  std::cout << "[Quantum Core] Application initialized" << std::endl;
  m_window = std::make_unique<Window>(800, 600, "Quantum Application");
}

void Application::run() {
  std::cout << "[Quantum Core] Application is running..." << std::endl;

  while (!m_window->shouldClose()) {
    m_window->pollEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    //
  }
}

void Application::shutdown() {
  std::cout << "[Quantum Core] Application shutting down" << std::endl;
  m_window.reset();
}

void Application::printMessage(const std::string &message) {
  std::cout << "[Quantum Core] Message: " << message << std::endl;
}

} // namespace core
} // namespace quantum
