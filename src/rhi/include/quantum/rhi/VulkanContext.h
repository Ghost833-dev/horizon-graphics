// src/rhi/include/quantum/rhi/VulkanContext.h

#pragma once

#include "quantum/core/Window.h"
#include <vulkan/vulkan.h>

namespace quantum {
namespace rhi {

class VulkanContext {
public:
  VulkanContext(quantum::core::Window &window);
  ~VulkanContext();

  VkInstance getInstance() const { return m_instance; }
  VkDevice getDevice() const { return m_device; }
  VkSurfaceKHR getSurface() const { return m_surface; }
  VkSwapchainKHR getSwapchain() const { return m_swapchain; }

  void createSwapchain(int width, int height);
  void destroySwapchain();

private:
  VkInstance m_instance{VK_NULL_HANDLE};
  VkDevice m_device{VK_NULL_HANDLE};
  VkSurfaceKHR m_surface{VK_NULL_HANDLE};
  VkSwapchainKHR m_swapchain{VK_NULL_HANDLE};
};

} // namespace rhi
} // namespace quantum
