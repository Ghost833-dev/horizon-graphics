// src/core/src/Application.cpp

#include <quantum/core/Application.h>
#include <iostream>

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
}

void Application::run() {
  std::cout << "[Quantum Core] Application is running..." << std::endl;
}

void Application::shutdown() {
  std::cout << "[Quantum Core] Application shutting down" << std::endl;
}

void Application::printMessage(const std::string& message) {
  std::cout << "[Quantum Core] Message: " << message << std::endl;
}

} // namespace core
} // namespace quantum
