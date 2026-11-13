// rhi/src/VulkanContext.cpp

#include <quantum/VulkanContext.h>
#include <quantum/Window.h>
#include <VkBootstrap.h>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace quantum {
namespace rhi {

VulkanContext::VulkanContext(quantum::core::Window &window)
    : m_window(window) {}

VulkanContext::~VulkanContext() {
  destroySwapchain();

  for (size_t i = 0; i < m_inFlightFences.size(); i++) {
    vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr);
    vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
    vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
  }

  if (m_commandPool != VK_NULL_HANDLE) {
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    m_commandPool = VK_NULL_HANDLE;
  }

  if (m_device != VK_NULL_HANDLE) {
    vkDestroyDevice(m_device, nullptr);
    m_device = VK_NULL_HANDLE;
  }

  if (m_surface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    m_surface = VK_NULL_HANDLE;
  }

  if (m_debugMessenger != VK_NULL_HANDLE) {
    vkb::destroy_debug_utils_messenger(m_instance, m_debugMessenger);
    m_debugMessenger = VK_NULL_HANDLE;
  }

  if (m_instance != VK_NULL_HANDLE) {
    vkDestroyInstance(m_instance, nullptr);
    m_instance = VK_NULL_HANDLE;
  }
}

void VulkanContext::initialize() {
  initVulkan();
  initSwapchain();
  initCommands();
  initSyncStructures();
  std::cout << "[Quantum RHI] Vulkan Context initialized." << std::endl;
}

void VulkanContext::initVulkan() {
  vkb::InstanceBuilder builder;
  auto inst_ret = builder.set_app_name("Quantum")
                      .request_validation_layers(true)
                      .use_default_debug_messenger()
                      .require_api_version(1, 3, 0)
                      .build();

  if (!inst_ret) {
    throw std::runtime_error("Failed to create Vulkan instance");
  }

  vkb::Instance vkb_inst = inst_ret.value();
  m_instance = vkb_inst.instance;
  m_debugMessenger = vkb_inst.debug_messenger;
  m_surface = m_window.createVulkanSurface(m_instance);

  VkPhysicalDeviceVulkan13Features features13{};
  features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
  features13.dynamicRendering = VK_TRUE;
  features13.synchronization2 = VK_TRUE;

  VkPhysicalDeviceVulkan12Features features12{};
  features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
  features12.bufferDeviceAddress = VK_TRUE;
  features12.descriptorIndexing = VK_TRUE;

  vkb::PhysicalDeviceSelector selector{vkb_inst};
  auto physicalDevice = selector.set_minimum_version(1, 3)
                            .set_required_features_13(features13)
                            .set_required_features_12(features12)
                            .set_surface(m_surface)
                            .select();

  if (!physicalDevice) {
    throw std::runtime_error("Failed to select Vulkan physical device");
  }

  m_physicalDevice = physicalDevice.value().physical_device;
  vkb::DeviceBuilder deviceBuilder{physicalDevice.value()};
  auto vkbDevice = deviceBuilder.build();

  if (!vkbDevice) {
    throw std::runtime_error("Failed to create Vulkan logical device");
  }

  m_device = vkbDevice.value().device;
}

void VulkanContext::initSwapchain() {
  createSwapchain(m_window.getWidth(), m_window.getHeight());
}

void VulkanContext::createSwapchain(int width, int height) {
  vkb::SwapchainBuilder swapchainBuilder{m_physicalDevice, m_device, m_surface};
  m_swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

  auto vkbSwapchain = swapchainBuilder
      .set_desired_format(VkSurfaceFormatKHR{
          .format = m_swapchainImageFormat,
          .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
      .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
      .set_desired_extent(static_cast<uint32_t>(width),
                          static_cast<uint32_t>(height))
      .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
      .build();

  if (!vkbSwapchain) {
    throw std::runtime_error("Failed to create Vulkan swapchain");
  }

  m_swapchain = vkbSwapchain.value().swapchain;
  m_swapchainExtent = vkbSwapchain.value().extent;
  m_swapchainImages = vkbSwapchain.value().get_images().value();
  m_swapchainImageViews = vkbSwapchain.value().get_image_views().value();
}

void VulkanContext::destroySwapchain() {
  if (m_device == VK_NULL_HANDLE || m_swapchain == VK_NULL_HANDLE) {
    return;
  }

  for (auto &imageView : m_swapchainImageViews) {
    if (imageView != VK_NULL_HANDLE) {
      vkDestroyImageView(m_device, imageView, nullptr);
    }
  }
  m_swapchainImageViews.clear();

  if (m_swapchain != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
    m_swapchain = VK_NULL_HANDLE;
  }
}

void VulkanContext::initCommands() {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create command pool");
  }

  m_commandBuffers.resize(m_swapchainImages.size());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = m_commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

  if (vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers");
  }
}

void VulkanContext::initSyncStructures() {
  const size_t MAX_FRAMES_IN_FLIGHT = 2;
  m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create synchronization objects");
    }
  }
}

} // namespace rhi
} // namespace quantum
