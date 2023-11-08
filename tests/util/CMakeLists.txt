set(SRCROOT ${SRCROOT}/util)

set(SRC
    ${SRCROOT}/tests_BitSet.cpp
    ${SRCROOT}/tests_StackAllocator.cpp
)

add_subdirectory(meta)
add_subdirectory(serialization)

add_ecstasy_test(
    NAME ecstasy_test_util
    DISPLAY_NAME "Ecstasy-Util-Tests"
    SOURCES ${SRC}
)
