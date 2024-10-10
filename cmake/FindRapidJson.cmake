
CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

if(NOT rapidjson_FOUND)
    include(FetchContent)

    FetchContent_Declare(rapidjson
        GIT_REPOSITORY https://github.com/Tencent/rapidjson
        GIT_TAG master
    )

    FetchContent_GetProperties(rapidjson)

    if(NOT rapidjson_POPULATED)
        SET(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(rapidjson)
        SET(RAPIDJSON_INCLUDE_PATH "${rapidjson_SOURCE_DIR}/include")
        SET(rapidjson_FOUND TRUE)
    endif()
endif()
