set(SRCROOT ${SRCROOT}/query)

set(SRC
    ${SRCROOT}/tests_Query.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_query
    DISPLAY_NAME "Ecstasy-Query-Tests"
    SOURCES ${SRC}
)
