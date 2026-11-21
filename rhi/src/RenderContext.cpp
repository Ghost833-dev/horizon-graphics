#include <memory>
#include <quantum/RenderContext.h>
#include <stdexcept>

#ifndef QUANTUM_RHI_VULKAN
#include <quantum/VulkanContext.h>
#endif
// #ifdef quantum_rhi_opengl
// #include <quantum/openglcontext.h>
// #endif

namespace quantum::rhi {

std::unique_ptr<RenderContext> RenderContext::create(quantum::core::Window &window) {
#ifdef QUANTUM_RHI_VULKAN
  return std::make_unique<VulkanContext>(window);
#elif defined(QUANTUM_RHI_OPENGL)
  // return std::make_unique<OpenGLContext>(window);
  throw std::runtime_error("OpenGL backend not implemented yet.");
#else
  throw std::runtime_error("No RHI backend selected during build!");
#endif
}
} // namespace quantum::rhi
