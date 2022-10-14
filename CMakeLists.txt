cmake_minimum_required(VERSION 3.19.0)

project(ecstasy LANGUAGES CXX)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(BUILD_TEST_SUITE OFF)
option(ENABLE_CODE_COVERAGE OFF)

# Add ./cmake to the module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(DetectCompiler)

set(ECSTASY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/")
file(MAKE_DIRECTORY ${ECSTASY_OUTPUT_DIRECTORY})


add_subdirectory(src/ecstasy)

# Unit testing
if(BUILD_TEST_SUITE)
    # Check for GoogleTest
    find_package(googletest REQUIRED)

    # Disable error on Linux release builds
    if(COMPILER_TYPE MATCHES "gcc")
        target_compile_options(gtest PRIVATE "-Wno-error=restrict")
        target_compile_options(gmock PRIVATE "-Wno-error=restrict")
    endif()

    enable_testing()

    add_subdirectory(tests)
endif()