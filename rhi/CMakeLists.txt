project(HorizonRHI)

set(VK_BOOTSTRAP_COMPILE_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/vk-bootstrap"
    "${CMAKE_CURRENT_BINARY_DIR}/vk-bootstrap-build"
    EXCLUDE_FROM_ALL
)

set_target_properties(vk-bootstrap PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

add_library(horizon_rhi SHARED
    src/VulkanContext.cpp
)

target_compile_definitions(horizon_rhi PRIVATE HORIZON_BUILD_SHARED)
target_include_directories(horizon_rhi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../core/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(horizon_rhi
    PUBLIC
        horizon_core
    PRIVATE
        Vulkan::Vulkan
        vk-bootstrap
)

set_target_properties(horizon_rhi PROPERTIES
    OUTPUT_NAME horizon_rhi
    VERSION ${Horizon_VERSION}
    SOVERSION ${Horizon_VERSION_MAJOR}
)

# Windows
if(WIN32)
  set_target_properties(horizon_rhi PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS OFF
    )
endif()

# APPLE
if(APPLE)
  target_link_libraries(horizon_rhi
        PRIVATE
            ${MOLTENVK_LIB}
            ${METAL_LIB}
            ${QUARTZCORE_LIB}
    )
  target_compile_definitions(horizon_rhi PRIVATE VK_USE_PLATFORM_METAL_EXT)
endif()
