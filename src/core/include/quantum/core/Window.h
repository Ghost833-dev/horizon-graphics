// src/core/include/quantum/core/Window.h

#pragma once

#include "src/Export.h"
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace quantum {
namespace core {

class QUANTUM_CORE_API Window {
public:
  Window(int width, int height, const std::string &title);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&other) noexcept;
  Window &operator=(Window &&other) noexcept;

  GLFWwindow *getHandle() const { return m_handle; }
  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }
  int getAspectRatio() const { return static_cast<float>(m_width) / m_height; }

  bool shouldClose() const;
  void pollEvents();

  VkSurfaceKHR createVulkanSurface(VkInstance instance);

private:
  void destroy();

  GLFWwindow *m_handle;
  int m_width;
  int m_height;
  std::string m_title;

  static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
};

} // namespace core
} // namespace quantum
