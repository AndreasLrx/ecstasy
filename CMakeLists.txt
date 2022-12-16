cmake_minimum_required(VERSION 3.19.0)

# Add ./cmake to the module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP
if(POLICY CMP0135)
	cmake_policy(SET CMP0135 NEW)
	set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

include(Options)
include(macros)

# Resolve dependencies before applying C++20 standards to avoid incompatibility
if (${ECSTASY_INTEGRATIONS_SFML})
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
endif()

if (${ECSTASY_SERIALIZER_TOML})
    find_package(Toml++)
    set(TomlPlusPlus_IncDir ${tomlplusplus_SOURCE_DIR}/include)
endif()

project(ecstasy LANGUAGES CXX)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(DetectCompiler)

set(ECSTASY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/")
file(MAKE_DIRECTORY ${ECSTASY_OUTPUT_DIRECTORY})

set(BUILDING_TESTS 0)
add_subdirectory(src)

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

    set(BUILDING_TESTS 1)
    add_subdirectory(tests)
endif()