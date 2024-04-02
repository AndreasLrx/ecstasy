set(SRCROOT ${SRCROOT}/thread)

set(SRC
    ${SRCROOT}/tests_Lockable.cpp
    ${SRCROOT}/tests_LockableView.cpp
    ${SRCROOT}/tests_SharedRecursiveMutex.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_thread
    DISPLAY_NAME "Ecstasy-Thread-Tests"
    SOURCES ${SRC}
)
