function(add_ecstasy_test)
    # Declare arguments
    set(oneValueArgs NAME DISPLAY_NAME WORKING_DIRECTORY)
    set(multiValueArgs INCLUDE_DIRS SOURCES)
    cmake_parse_arguments(add_ecstasy_test "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Check for required arguments NAME and SOURCES
    if(NOT DEFINED add_ecstasy_test_NAME)
        message(FATAL_ERROR "missing unit test name")
    endif()

    if(NOT DEFINED add_ecstasy_test_SOURCES)
        message(FATAL_ERROR "missing unit test sources")
    endif()

    # Define shorthand variables
    set(TARGET_NAME ${add_ecstasy_test_NAME})
    set(SRC ${add_ecstasy_test_SOURCES})
    set(INCDIRS ${add_ecstasy_test_INCLUDE_DIRS})
    set(WORKING_DIRECTORY ${add_ecstasy_test_WORKING_DIRECTORY})

    if(DEFINED add_ecstasy_test_DISPLAY_NAME)
        set(DISPLAY_NAME ${add_ecstasy_test_DISPLAY_NAME})
    else()
        set(DISPLAY_NAME ${TARGET_NAME})
    endif()

    # Create the test executable
    add_executable(${TARGET_NAME} ${SRC})
    target_include_directories(${TARGET_NAME} PRIVATE ${INCROOT} ${INCDIRS})

    # Add compiler flags
    ecstasy_set_compiler_flags(${TARGET_NAME})

    source_group(${TARGET_NAME} FILES ${SRC})
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "tests")

    target_link_libraries(${TARGET_NAME} PRIVATE ecstasy)
    target_link_libraries_system(${TARGET_NAME} PRIVATE gmock_main)

    # Register the test
    add_test(NAME "${DISPLAY_NAME}" COMMAND ${TARGET_NAME} WORKING_DIRECTORY ${WORKING_DIRECTORY})
endfunction()

set(INCROOT ${PROJECT_SOURCE_DIR}/tests)
set(SRCROOT ${PROJECT_SOURCE_DIR}/tests)

add_subdirectory(entity)
add_subdirectory(query)
add_subdirectory(storage)
add_subdirectory(registry)
add_subdirectory(resources)
add_subdirectory(rtti)
add_subdirectory(serialization)
add_subdirectory(thread)
add_subdirectory(util)
add_subdirectory(integration)
