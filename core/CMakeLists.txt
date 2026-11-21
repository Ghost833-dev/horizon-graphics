# core/CMakeLists.txt

project(HorizonCore)

add_library(horizon_core SHARED
    src/Application.cpp
    src/Window.cpp
)

target_compile_definitions(horizon_core PRIVATE HORIZON_BUILD_SHARED)
target_include_directories(horizon_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(horizon_core
    PUBLIC
        glfw
    PRIVATE
        Vulkan::Vulkan
)

set_target_properties(horizon_core PROPERTIES
    OUTPUT_NAME horizon_core
    VERSION ${Horizon_VERSION}
    SOVERSION ${Horizon_VERSION_MAJOR}
)

# Windows
if(WIN32)
  set_target_properties(horizon_core PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS OFF
    )
endif()

# APPLE
if(APPLE)
  target_link_libraries(horizon_core
        PRIVATE
            ${MOLTENVK_LIB}
            ${METAL_LIB}
            ${QUARTZCORE_LIB}
    )
  target_compile_definitions(horizon_core PRIVATE VK_USE_PLATFORM_METAL_EXT)
endif()
