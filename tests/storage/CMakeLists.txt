set(SRCROOT ${SRCROOT}/storage)

set(SRC
    ${SRCROOT}/tests_Instances.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_storages
    DISPLAY_NAME "Ecstasy-Storage-Tests"
    SOURCES ${SRC}
)
