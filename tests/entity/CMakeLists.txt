set(SRCROOT ${SRCROOT}/entity)

set(SRC
    ${SRCROOT}/tests_Entities.cpp
    ${SRCROOT}/tests_Entity.cpp
    ${SRCROOT}/tests_RegistryEntity.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_entity
    DISPLAY_NAME "Ecstasy-Entity-Tests"
    SOURCES ${SRC}
)
