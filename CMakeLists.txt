cmake_minimum_required(VERSION 3.16.0)

project(ecstasy LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add ./cmake to the module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(DetectCompiler)
include(CompilerFlags)

set(ECSTASY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/")
file(MAKE_DIRECTORY ${ECSTASY_OUTPUT_DIRECTORY})


add_subdirectory(src/ecstasy)
