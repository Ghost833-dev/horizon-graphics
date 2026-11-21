#pragma once
#include <memory>
#include <quantum/Window.h>

namespace quantum::rhi {

class RenderContext {
public:
  virtual ~RenderContext() = default;

  virtual void initialize() = 0;
  virtual void beginFrame() = 0;
  virtual void endFrame() = 0;
  virtual void waitIdle() = 0;

  static std::unique_ptr<RenderContext> create(quantum::core::Window &window);
};
} // namespace quantum::rhi
