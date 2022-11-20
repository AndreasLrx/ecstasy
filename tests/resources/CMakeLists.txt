set(SRCROOT ${SRCROOT}/resources)

set(SRC
    ${SRCROOT}/tests_ObjectWrapper.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_resources
    DISPLAY_NAME "Ecstasy-Resources-Tests"
    SOURCES ${SRC}
)
