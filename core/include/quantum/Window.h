// core/include/quantum/Window.h

#pragma once
#include <quantum/Export.h>
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
  bool isMinimized() const { return m_isMinimized; }
  bool isMaximized() const { return m_isMaximized;}

  VkSurfaceKHR createVulkanSurface(VkInstance instance);

private:
  void destroy();
  void setupCallbacks();

  GLFWwindow *m_handle = nullptr;
  int m_width;
  int m_height;
  std::string m_title;
  bool m_isMinimized = false;
  bool m_isMaximized = false;

  static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
  static void windowIconifyCallback(GLFWwindow* window, int iconified);
  static void windowMaximizeCallback(GLFWwindow* window, int maximized);
};

} // namespace core
} // namespace quantum
