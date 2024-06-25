set(SRCROOT ${SRCROOT}/serialization)

set(SRC
    ${SRCROOT}/tests_ComponentSerializer.cpp
    ${SRCROOT}/tests_concepts.cpp
    ${SRCROOT}/tests_Serializer.cpp
    ${SRCROOT}/tests_traits.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_serialization
    DISPLAY_NAME "Ecstasy-Serialization-Tests"
    SOURCES ${SRC}
)
