set(SRCROOT ${SRCROOT}/event)

set(SRC
    ${SRCROOT}/tests_Keyboard.cpp
    ${SRCROOT}/tests_Mouse.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_event
    DISPLAY_NAME "Ecstasy-Event-Tests"
    SOURCES ${SRC}
)
