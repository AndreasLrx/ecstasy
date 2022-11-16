set(INCROOT ${INCROOT}/sfml)
set(SRCROOT ${SRCROOT}/sfml)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

add_subdirectory(demo)
add_subdirectory(resources)
add_subdirectory(systems)

set(SRC 
    ${SRC}
    PARENT_SCOPE)

set(SYSTEM_INCDIRS 
    ${SYSTEM_INCDIRS} 
    ${sfml_SOURCE_DIR}/include
    PARENT_SCOPE)