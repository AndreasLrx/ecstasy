cmake_minimum_required(VERSION 3.17)

include(CMakeDependentOption)

option(BUILD_TEST_SUITE "Whether the test suite must be built." OFF)
option(ENABLE_TEST_COVERAGE "Whether the tests code coverage must be enabled" OFF)
option(BUILD_SHARED_LIBS "Whether ecstasy must be built as a shared library or not" OFF)

## Multi-Threading options
option(ECSTASY_THREAD_SAFE "Enable thread safety for all lockable resources and storages. 
This is the combination of ECSTASY_AUTO_LOCK, ECSTASY_LOCKABLE_RESOURCES and  ECSTASY_LOCKABLE_STORAGES." OFF)

option(ECSTASY_AUTO_LOCK "Registry queries and modifiers (RegistryModifier) lock Lockable queryables by default (implicit thread safety). 
This doesn't affect the default false value in the ecstasy::query namespace. (QueryImplementation and ecstasy::query::modifier::*)" OFF)
option(ECSTASY_LOCKABLE_RESOURCES "Resources inherit from SharedRecursiveMutex and are therefore lockable." OFF)
option(ECSTASY_LOCKABLE_STORAGES "Storages inherit from SharedRecursiveMutex and are therefore lockable." OFF)

if (${ECSTASY_THREAD_SAFE})
    set(ECSTASY_AUTO_LOCK ON)
    set(ECSTASY_LOCKABLE_RESOURCES ON)
    set(ECSTASY_LOCKABLE_STORAGES ON)
endif()

## Integration libraries
option(ECSTASY_INTEGRATIONS_EVENT "Events managing integration. Include event listeners and input states (mouse, keyboard, gamepad)." OFF)
CMAKE_DEPENDENT_OPTION(ECSTASY_INTEGRATIONS_SFML "SFML library integration." OFF ECSTASY_INTEGRATIONS_EVENT OFF)
CMAKE_DEPENDENT_OPTION(ECSTASY_INTEGRATIONS_SFML_BUILD_DEMO "Build SFML integration demo projects." OFF ECSTASY_INTEGRATIONS_SFML OFF)
CMAKE_DEPENDENT_OPTION(ECSTASY_INTEGRATIONS_USER_ACTION "User Action integration." OFF ECSTASY_INTEGRATIONS_EVENT OFF)

## Serializer formats
option(ECSTASY_SERIALIZER_TOML "Build Toml serializer." OFF)

if (${ECSTASY_INTEGRATIONS_USER_ACTION})
    set(ECSTASY_SERIALIZER_TOML ON)
endif()


## Display configuration
message(STATUS "ECSTASY Options:")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
    message(STATUS "Build Tests: ${BUILD_TEST_SUITE}")
    message(STATUS "Enable test coverage: ${ENABLE_TEST_COVERAGE}")
    message(STATUS "Build shared libraries: ${BUILD_SHARED_LIBS}")
    ## Multi-Threading
    message(STATUS "Multi-Threading:")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
        message(STATUS "Auto lock: ${ECSTASY_AUTO_LOCK}")
        message(STATUS "Lockable resources: ${ECSTASY_LOCKABLE_RESOURCES}")
        message(STATUS "Lockable storages: ${ECSTASY_LOCKABLE_STORAGES}")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
    ## Integrations
    message(STATUS "Integrations:")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
        message(STATUS "Event: ${ECSTASY_INTEGRATIONS_EVENT}")
        message(STATUS "SFML: ${ECSTASY_INTEGRATIONS_SFML}")
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
            message(STATUS "Build demos: ${ECSTASY_INTEGRATIONS_SFML_BUILD_DEMO}")
        list(POP_BACK CMAKE_MESSAGE_INDENT)
        message(STATUS "User Action: ${ECSTASY_INTEGRATIONS_USER_ACTION}")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
    ## Serializers
    message(STATUS "Serializes:")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
        message(STATUS "Toml: ${ECSTASY_SERIALIZER_TOML}")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
list(POP_BACK CMAKE_MESSAGE_INDENT)
