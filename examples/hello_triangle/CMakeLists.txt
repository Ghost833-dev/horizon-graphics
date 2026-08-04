# examples/hello_triangle/CMakeLists.txt
project(HelloTriangle)

add_executable(hello_triangle main.cpp)

target_link_libraries(hello_triangle
    PRIVATE
        quantum_core
        quantum_rhi
)

# RPATH for Linux/macOS
if(UNIX)
  get_target_property(CORE_BINARY_DIR quantum_core BINARY_DIR)
  set_target_properties(hello_triangle PROPERTIES
        BUILD_RPATH "${CORE_BINARY_DIR}"
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif()

# Windows
if(WIN32)
  add_custom_command(TARGET hello_triangle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:quantum_core>
            $<TARGET_FILE_DIR:hello_triangle>
        COMMENT "Copying quantum_core.dll to output directory"
    )
endif()
