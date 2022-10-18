set(SRCROOT ${SRCROOT}/utils)

set(SRC
    ${SRCROOT}/tests_BitSet.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_utils
    DISPLAY_NAME "Ecstasy-Utils-Tests"
    SOURCES ${SRC}
)
