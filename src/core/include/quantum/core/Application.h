// src/core/include/quantum/core/Application.h

#pragma once
#include "src/Export.h"
#include <memory>

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

private:
  std::unique_ptr<Window> m_window;
};

} // namespace core
} // namespace quantum
