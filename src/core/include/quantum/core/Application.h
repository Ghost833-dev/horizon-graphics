// src/core/include/quantum/core/Application.h

#pragma once
#include <string>
#include "src/Export.h"

namespace quantum {
namespace core {

class QUANTUM_CORE_API Application {
public:
  Application();
  ~Application();

  void initialize();
  void run();
  void shutdown();
  void printMessage(const std::string& message);
};

} // namespace core
} // namespace quantum
