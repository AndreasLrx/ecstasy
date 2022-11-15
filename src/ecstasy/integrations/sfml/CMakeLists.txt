set(INCROOT ${INCROOT}/sfml)
set(SRCROOT ${SRCROOT}/sfml)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

add_subdirectory(demo)

set(SRC 
    ${SRC}
    PARENT_SCOPE)