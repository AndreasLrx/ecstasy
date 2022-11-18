cmake_minimum_required(VERSION 3.17)

include(CMakeDependentOption)

option(BUILD_TEST_SUITE "Whether the test suite must be built." OFF)
option(ENABLE_CODE_COVERAGE "Whether the code coverage must be enabled" OFF)
option(BUILD_SHARED_LIBS "Whether ecstasy must be built as a shared library or not" OFF)

## Integration libraries
option(ECSTASY_INTEGRATIONS_EVENT "Events managing integration. Include event listeners and input states (mouse, keyboard, gamepad)." OFF)
CMAKE_DEPENDENT_OPTION(ECSTASY_INTEGRATIONS_SFML "SFML library integration." OFF ECSTASY_INTEGRATIONS_EVENT OFF)
CMAKE_DEPENDENT_OPTION(ECSTASY_INTEGRATIONS_SFML_BUILD_DEMO "Build SFML integration demo projects." OFF ECSTASY_INTEGRATIONS_SFML OFF)


## Display configuration
message(STATUS "ECSTASY Options:")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
    message(STATUS "Build Tests: ${BUILD_TEST_SUITE}")
    message(STATUS "Enable code Coverage: ${ENABLE_CODE_COVERAGE}")
    message(STATUS "Build shared libraries: ${BUILD_SHARED_LIBS}")
    ## Integrations
    message(STATUS "Build integrations")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
        message(STATUS "Event: ${ECSTASY_INTEGRATIONS_EVENT}")
        message(STATUS "SFML: ${ECSTASY_INTEGRATIONS_SFML}")
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
            message(STATUS "Build demos: ${ECSTASY_INTEGRATIONS_SFML_BUILD_DEMO}")
        list(POP_BACK CMAKE_MESSAGE_INDENT)
    list(POP_BACK CMAKE_MESSAGE_INDENT)
list(POP_BACK CMAKE_MESSAGE_INDENT)
