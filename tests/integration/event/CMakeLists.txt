set(SRCROOT ${SRCROOT}/event)

set(SRC
    ${SRCROOT}/tests_Gamepad.cpp
    ${SRCROOT}/tests_Keyboard.cpp
    ${SRCROOT}/tests_Mouse.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_integration_event
    DISPLAY_NAME "Ecstasy-Integration-Event-Tests"
    SOURCES ${SRC}
)
