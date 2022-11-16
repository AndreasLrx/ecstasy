
CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

if(NOT sfml_FOUND)
    include(FetchContent)

    FetchContent_Declare(sfml
        URL https://github.com/SFML/SFML/archive/refs/tags/2.5.1.tar.gz
        URL_HASH MD5=0d8a10fad7f10fa7b4453e647fb6b1a8)

    FetchContent_GetProperties(sfml)

    if(NOT sfml_POPULATED)
        SET(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(sfml)
        ADD_SUBDIRECTORY(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
        SET(sfml_FOUND TRUE)
    endif()
endif()