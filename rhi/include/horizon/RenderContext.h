#pragma once
#include <memory>
#include <horizon/Window.h>

namespace horizon::rhi {

class RenderContext {
public:
  virtual ~RenderContext() = default;

  virtual void initialize() = 0;
  virtual void beginFrame() = 0;
  virtual void endFrame() = 0;
  virtual void waitIdle() = 0;

  static std::unique_ptr<RenderContext> create(horizon::core::Window &window);
};
} // namespace horizon::rhi
