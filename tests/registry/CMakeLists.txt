set(SRCROOT ${SRCROOT}/registry)

set(SRC
    ${SRCROOT}/tests_DeletionStack.cpp
    ${SRCROOT}/tests_queryable_type.cpp
    ${SRCROOT}/tests_Registry.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_registry
    DISPLAY_NAME "Ecstasy-Registry-Tests"
    SOURCES ${SRC}
)
