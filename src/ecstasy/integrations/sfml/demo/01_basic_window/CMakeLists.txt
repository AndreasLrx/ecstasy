set(INCROOT ${INCROOT}/01_basic_window)
set(SRCROOT ${SRCROOT}/01_basic_window)


set(SRC
    ${SRCROOT}/main.cpp)

add_sfml_demo(
    NAME basic_window
    SOURCES ${SRC}
    INCLUDE_DIRS ${INCROOT})