# src/rhi/CMakeLists.txt

project(QuantumRHI)

add_library(quantum_rhi SHARED
    src/VulkanDevice.cpp
)

target_compile_definitions(quantum_rhi PRIVATE QUANTUM_BUILD_SHARED)
target_include_directories(quantum_rhi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(quantum_rhi
    PUBLIC
        quantum_core
    PRIVATE
        Vulkan::Vulkan
)

set_target_properties(quantum_rhi PROPERTIES
    OUTPUT_NAME quantum_rhi
    VERSION ${Quantum_VERSION}
    SOVERSION ${Quantum_VERSION_MAJOR}
)

# Windows
if(WIN32)
  set_target_properties(quantum_rhi PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS OFF
    )
endif()

# APPLE
if(APPLE)
  target_link_libraries(quantum_rhi
        PRIVATE
            ${MOLTENVK_LIB}
            ${METAL_LIB}
            ${QUARTZCORE_LIB}
    )
  target_compile_definitions(quantum_rhi PRIVATE VK_USE_PLATFORM_METAL_EXT)
endif()
