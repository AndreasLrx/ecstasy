cmake_minimum_required(VERSION 3.14)

option(BUILD_TEST_SUITE "Whether the test suite must be built." OFF)
option(ENABLE_CODE_COVERAGE "Whether the code coverage must be enabled" OFF)

## Integration libraries
option(ECSTASY_INTEGRATIONS_EVENT "Events managing integration. Include event listeners and input states (mouse, keyboard, gamepad)." OFF)