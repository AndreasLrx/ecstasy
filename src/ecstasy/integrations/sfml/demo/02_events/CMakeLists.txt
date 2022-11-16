set(INCROOT ${INCROOT}/02_events)
set(SRCROOT ${SRCROOT}/02_events)


set(SRC
    ${SRCROOT}/main.cpp)

add_sfml_demo(
    NAME events
    SOURCES ${SRC}
    INCLUDE_DIRS ${INCROOT})