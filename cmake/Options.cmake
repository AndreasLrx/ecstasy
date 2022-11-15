cmake_minimum_required(VERSION 3.14)

option(BUILD_TEST_SUITE "Whether the test suite must be built." OFF)
option(ENABLE_CODE_COVERAGE "Whether the code coverage must be enabled" OFF)
option(BUILD_SHARED_LIBS "Whether ecstasy must be built as a shared library or not" OFF)

## Integration libraries
option(ECSTASY_INTEGRATIONS_EVENT "Events managing integration. Include event listeners and input states (mouse, keyboard, gamepad)." OFF)
option(ECSTASY_INTEGRATIONS_SFML "SFML library integration." OFF)