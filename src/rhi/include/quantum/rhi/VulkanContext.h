// src/rhi/include/quantum/rhi/VulkanContext.h

#pragma once

#include "quantum/core/Window.h"
#include <cstddef>
#include <vector>
#include <vulkan/vulkan.h>

namespace quantum {
namespace rhi {

class VulkanContext {
public:
  explicit VulkanContext(quantum::core::Window &window);
  ~VulkanContext();

  VulkanContext(const VulkanContext &) = delete;
  VulkanContext &operator=(const VulkanContext &) = delete;

  VkInstance getInstance() const { return m_instance; }
  VkDevice getDevice() const { return m_device; }
  VkSurfaceKHR getSurface() const { return m_surface; }
  VkSwapchainKHR getSwapchain() const { return m_swapchain; }
  VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }

  void initialize();

  void createSwapchain(int width, int height);
  void destroySwapchain();
  VkExtent2D getSwapchainExtent() const { return m_swapchainExtent; }
  VkFormat getSwapchainImageFormat() const { return m_swapchainImageFormat; }
  const std::vector<VkImage> &getSwapchainImages() const { return m_swapchainImages; }
  const std::vector<VkImageView> &getSwapchainImageViews() const { return m_swapchainImageViews; }

private:
  void initVulkan();
  void initSwapchain();
  void initCommands();
  void initSyncStructures();

  quantum::core::Window &m_window;

  VkInstance m_instance{VK_NULL_HANDLE};
  VkDebugUtilsMessengerEXT m_debugMessenger{VK_NULL_HANDLE};
  VkDevice m_device{VK_NULL_HANDLE};
  VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
  VkSurfaceKHR m_surface{VK_NULL_HANDLE};

  VkSwapchainKHR m_swapchain{VK_NULL_HANDLE};
  VkFormat m_swapchainImageFormat{VK_FORMAT_UNDEFINED};
  VkExtent2D m_swapchainExtent{};
  std::vector<VkImage> m_swapchainImages;
  std::vector<VkImageView> m_swapchainImageViews;

  VkCommandPool m_commandPool{VK_NULL_HANDLE};
  std::vector<VkCommandBuffer> m_commandBuffers;
  std::vector<VkSemaphore> m_imageAvailableSemaphores;
  std::vector<VkSemaphore> m_renderFinishedSemaphores;
  std::vector<VkFence> m_inFlightFences;
  size_t m_currentFrame{0};
};

} // namespace rhi
} // namespace quantum
