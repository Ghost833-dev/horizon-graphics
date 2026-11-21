cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME Horizon)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform-specific settings

# Search Vulkan
find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
  message(STATUS "Vulkan found: ${Vulkan_INCLUDE_DIRS}")
else()
  message(FATAL_ERROR "Vulkan not found. Please install Vulkan SDK.")
endif()

# Search GLFW
find_package(glfw3 REQUIRED)
if(glfw3_FOUND)
  message(STATUS "GLFW found: ${glfw3_INCLUDE_DIRS}")
else()
  message(FATAL_ERROR "GLFW not found. Please install GLFW.")
endif()

# Windows
if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# macOS: MoltenVK
if(APPLE)
  add_compile_definitions(VK_USE_PLATFORM_METAL_EXT)
  find_library(METAL_LIB Metal REQUIRED)
  find_library(QUARTZCORE_LIB QuartzCore REQUIRED)
  set(MOLTENVK_DIR "/usr/local/lib" CACHE PATH "Path to MoltenVK")
  find_library(MOLTENVK_LIB MoltenVK PATHS ${MOLTENVK_DIR})
  if(NOT MOLTENVK_LIB)
    message(WARNING "MoltenVK not found. Install with: brew install molten-vk")
  endif()
endif()

add_subdirectory(core)
add_subdirectory(rhi)

# Examples
option(HORIZON_BUILD_EXAMPLES "Build examples" ON)
if(HORIZON_BUILD_EXAMPLES)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples)
    add_subdirectory(examples)
    message(STATUS "Examples enabled")
  else()
    message(WARNING "Examples folder not found at: ${CMAKE_CURRENT_SOURCE_DIR}/examples")
  endif()
endif()
