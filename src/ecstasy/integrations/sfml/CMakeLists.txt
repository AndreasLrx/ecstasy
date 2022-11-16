set(INCROOT ${INCROOT}/sfml)
set(SRCROOT ${SRCROOT}/sfml)

add_subdirectory(demo)
add_subdirectory(resources)
add_subdirectory(systems)

set(SRC 
    ${SRC}
    PARENT_SCOPE)