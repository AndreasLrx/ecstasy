set(SRCROOT ${SRCROOT}/storage)

set(SRC
    ${SRCROOT}/tests_Instances.cpp
    ${SRCROOT}/tests_MapStorage.cpp
    ${SRCROOT}/tests_MarkerStorage.cpp
    ${SRCROOT}/tests_VectorStorage.cpp
    ${SRCROOT}/tests_StorageConcept.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_storages
    DISPLAY_NAME "Ecstasy-Storage-Tests"
    SOURCES ${SRC}
)
