#pragma once
#include <horizon/Export.h>
#include <memory>

namespace horizon {
namespace core {

class Window;

class HORIZON_CORE_API Application {
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
} // namespace horizon
