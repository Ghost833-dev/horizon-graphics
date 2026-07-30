// src/core/include/quantum/core/Application.h

#pragma once
#include "src/Export.h"
#include <memory>
#include <string>

namespace quantum {
namespace core {

class Window;

class QUANTUM_CORE_API Application {
public:
  Application();
  ~Application();

  void initialize();
  void run();
  void shutdown();
  void printMessage(const std::string &message);

private:
  std::unique_ptr<Window> m_window;
};

} // namespace core
} // namespace quantum
