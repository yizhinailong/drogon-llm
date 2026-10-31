file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp
)

foreach(TEST_SOURCE IN LISTS TEST_SOURCES)
    cmake_path(GET TEST_SOURCE STEM TEST_NAME)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
    target_compile_features(${TEST_NAME} PRIVATE cxx_std_23)
    target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

    # Tests load config.json from the repo root. The app-backed ones share its
    # listener port, so the lock keeps them apart under ctest -j. Exit code 77
    # reports a test as skipped.
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
    set_tests_properties(
        ${TEST_NAME}
        PROPERTIES
        RESOURCE_LOCK config_json_listener
        SKIP_RETURN_CODE 77
        TIMEOUT 60
    )
endforeach()

# test_deepseek calls the real API with a 120-second request timeout, and only
# runs when DEEPSEEK_API_KEY is set. ctest -LE live leaves it out.
set_tests_properties(
    test_deepseek
    PROPERTIES
    LABELS live
    TIMEOUT 300
)
