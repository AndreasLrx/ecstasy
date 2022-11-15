set(INCROOT ${INCROOT}/01_basic_window)
set(SRCROOT ${SRCROOT}/01_basic_window)

include(CompilerFlags)


set(SRC
    ${SRCROOT}/main.cpp)

set(DEMO_NAME basic_window)

add_executable(${DEMO_NAME} ${SRC})

target_include_directories(${DEMO_NAME} PRIVATE ${INCROOT})
target_link_libraries(${DEMO_NAME} PRIVATE ecstasy)

add_custom_command(TARGET ${DEMO_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${DEMO_NAME}> ${SFML_DEMO_OUTPUT_DIRECTORY}/)