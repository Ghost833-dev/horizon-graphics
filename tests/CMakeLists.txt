project(HorizonTests)

add_executable(horizon_tests
    main_tests.cpp
)

target_link_libraries(horizon_tests
    PRIVATE
        horizon_core
        horizon_rhi
        GTest::gtest_main
)

if(UNIX)
  get_target_property(CORE_BINARY_DIR horizon_core BINARY_DIR)
  get_target_property(RHI_BINARY_DIR horizon_rhi BINARY_DIR)
  set_target_properties(horizon_tests PROPERTIES
        BUILD_RPATH "${CORE_BINARY_DIR}:${RHI_BINARY_DIR}"
    )
endif()

include(GoogleTest)
gtest_discover_tests(horizon_tests)
