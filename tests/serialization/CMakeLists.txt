set(SRCROOT ${SRCROOT}/serialization)

set(SRC
    ${SRCROOT}/tests_Serializer.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_serialization
    DISPLAY_NAME "Ecstasy-Serialization-Tests"
    SOURCES ${SRC}
)
