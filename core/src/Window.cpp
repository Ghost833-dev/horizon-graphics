// core/src/Window.cpp

#include <quantum/Window.h>
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace quantum {
namespace core {

Window::Window(int width, int height, const std::string &title)
  : m_width(width), m_height(height), m_title(title), m_handle(nullptr) {

  static bool glfwInitialized = false;
  if (!glfwInitialized) {
    if (!glfwInit()) {
      throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwInitialized = true;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  m_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!m_handle) {
    throw std::runtime_error("Failed to create GLFW window");
  }

  setupCallbacks();
}

Window::~Window() {
  destroy();
}

Window::Window(Window &&other) noexcept
  : m_handle(other.m_handle), m_width(other.m_width),
    m_height(other.m_height), m_title(std::move(other.m_title)) {
  other.m_handle = nullptr;
  if (m_handle) {
    glfwSetWindowUserPointer(m_handle,this);
  }
}

Window &Window::operator=(Window  &&other) noexcept {
  if (this != &other) {
    destroy();
    m_handle = other.m_handle;
    m_width = other.m_width;
    m_height = other.m_height;
    m_title = std::move(other.m_title);
    other.m_handle = nullptr;
    if (m_handle) {
      glfwSetWindowUserPointer(m_handle, this);
    }
  }
  return *this;
}

bool Window::shouldClose() const {
  return m_handle && glfwWindowShouldClose(m_handle);
}

void Window::pollEvents() {
  glfwPollEvents();
}

VkSurfaceKHR Window::createVulkanSurface(VkInstance instance) {
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  if (glfwCreateWindowSurface(instance, m_handle, nullptr, &surface) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan surface");
  }
  return surface;
}

void Window::destroy() {
  if (m_handle) {
    glfwDestroyWindow(m_handle);
    m_handle = nullptr;
  }
}

void Window::setupCallbacks() {
  glfwSetWindowUserPointer(m_handle, this);
  glfwSetFramebufferSizeCallback(m_handle, framebufferSizeCallback);
  glfwSetKeyCallback(m_handle, keyCallback);
  glfwSetMouseButtonCallback(m_handle, mouseButtonCallback);
  glfwSetCursorPosCallback(m_handle, cursorPosCallback);
  glfwSetWindowIconifyCallback(m_handle, windowIconifyCallback);
  glfwSetWindowMaximizeCallback(m_handle, windowMaximizeCallback);
}

void Window::windowIconifyCallback(GLFWwindow *window, int iconified) {
  auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (self) {
    self->m_isMinimized = (iconified == GLFW_TRUE);
  }
}

void Window::windowMaximizeCallback(GLFWwindow *window, int maximized) {
  auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (self) {
    self->m_isMaximized = (maximized == GLFW_TRUE);
  }
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (self) {
    self->m_width = width;
    self->m_height = height;
  }
}

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (self) {
    //
  }
}

void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (self) {
    //
  }
}

void Window::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (self) {
    //
  }
}

} // namespace core
} // namespace quantum
