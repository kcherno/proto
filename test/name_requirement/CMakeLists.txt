set(PROJECT_TEST_FILES test_container.cpp)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

foreach(TEST_FILE ${PROJECT_TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})

    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    target_link_libraries(${TEST_NAME} PRIVATE Boost::unit_test_framework)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} --log_level=all)

endforeach()
