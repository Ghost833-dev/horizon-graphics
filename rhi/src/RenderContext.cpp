#include <memory>
#include <horizon/RenderContext.h>
#include <stdexcept>

#ifndef HORIZON_RHI_VULKAN
#include <horizon/VulkanContext.h>
#endif
// #ifdef horizon_rhi_opengl
// #include <horizon/openglcontext.h>
// #endif

namespace horizon::rhi {

std::unique_ptr<RenderContext> RenderContext::create(horizon::core::Window &window) {
#ifdef HORIZON_RHI_VULKAN
  return std::make_unique<VulkanContext>(window);
#elif defined(HORIZON_RHI_OPENGL)
  // return std::make_unique<OpenGLContext>(window);
  throw std::runtime_error("OpenGL backend not implemented yet.");
#else
  throw std::runtime_error("No RHI backend selected during build!");
#endif
}
} // namespace horizon::rhi
