set(SRCROOT ${SRCROOT}/rtti)

set(SRC
    ${SRCROOT}/tests_Type.cpp
    ${SRCROOT}/tests_TypeRegistry.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_rtti
    DISPLAY_NAME "Ecstasy-RTTI-Tests"
    SOURCES ${SRC}
)
