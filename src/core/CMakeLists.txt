# src/core/CMakeLists.txt

project(QuantumCore)

add_library(quantum_core SHARED
    src/Application.cpp
)

target_compile_definitions(quantum_core PRIVATE QUANTUM_BUILD_SHARED)
target_include_directories(quantum_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(quantum_core PROPERTIES
    OUTPUT_NAME quantum_core
    VERSION ${Quantum_VERSION}
    SOVERSION ${Quantum_VERSION_MAJOR}
)

# Windows
if(WIN32)
    set_target_properties(quantum_core PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS OFF
    )
endif()

# APPLE
if(APPLE)
    target_link_libraries(quantum_core
        PRIVATE
            ${MOLTENVK_LIB}
            ${METAL_LIB}
            ${QUARTZCORE_LIB}
    )
    target_compile_definitions(quantum_core PRIVATE VK_USE_PLATFORM_METAL_EXT)
endif()