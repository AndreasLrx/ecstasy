set(INCROOT ${INCROOT}/demo)
set(SRCROOT ${SRCROOT}/demo)

set(SFML_DEMO_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/demo/sfml")
file(MAKE_DIRECTORY ${SFML_DEMO_OUTPUT_DIRECTORY})


function(add_sfml_demo)
    # Declare arguments
    set(oneValueArgs NAME)
    set(multiValueArgs INCLUDE_DIRS SOURCES)
    cmake_parse_arguments(add_sfml_demo "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Check for required arguments NAME and SOURCES
    if(NOT DEFINED add_sfml_demo_NAME)
        message(FATAL_ERROR "missing demo name")
    endif()

    if(NOT DEFINED add_sfml_demo_SOURCES)
        message(FATAL_ERROR "missing demo sources")
    endif()

    set(DEMO_NAME ${add_sfml_demo_NAME})
    set(SRC ${add_sfml_demo_SOURCES})
    set(INCROOT ${add_sfml_demo_INCLUDE_DIRS})

    add_executable(${DEMO_NAME} ${SRC})
    ecstasy_set_compiler_flags(${DEMO_NAME})

    target_include_directories(${DEMO_NAME} PRIVATE ${INCROOT})
    target_link_libraries(${DEMO_NAME} PRIVATE ecstasy)
    target_link_libraries_system(${DEMO_NAME} PRIVATE sfml-graphics sfml-window sfml-system)

    add_custom_command(TARGET ${DEMO_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${DEMO_NAME}> ${SFML_DEMO_OUTPUT_DIRECTORY}/)
endfunction()

add_subdirectory(01_basic_window)
add_subdirectory(02_events)
