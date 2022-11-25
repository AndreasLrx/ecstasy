CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

if(NOT tomlplusplus_FOUND)
    FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG        v3.2.0)

    FetchContent_GetProperties(tomlplusplus)

    if(NOT tomlplusplus_POPULATED)
        SET(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(tomlplusplus)
        ADD_SUBDIRECTORY(${tomlplusplus_SOURCE_DIR} ${tomlplusplus_BINARY_DIR})
        SET(tomlplusplus_FOUND TRUE)
    endif()
endif()