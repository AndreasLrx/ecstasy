set(SRCROOT ${PROJECT_SOURCE_DIR}/src/ecstasy)
set(INCROOT ${PROJECT_SOURCE_DIR}/src/ecstasy)

set(SRC
    return_input.cpp)
set(INCDIRS ${INCROOT})


# Source groups are a feature in Visual Studio.
source_group("" FILES ${SRC})

set(TARGET_NAME ecstasy)
set(LIB_NAME ${TARGET_NAME})

include(CompilerFlags)

add_library(${LIB_NAME} ${SRC})
target_include_directories(${LIB_NAME} PRIVATE ${INCDIRS})
target_include_directories(${LIB_NAME} PUBLIC ${INCROOT})

add_custom_command(TARGET ${LIB_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIB_NAME}> ${ECSTASY_OUTPUT_DIRECTORY})
