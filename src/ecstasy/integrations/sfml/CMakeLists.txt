set(INCROOT ${INCROOT}/sfml)
set(SRCROOT ${SRCROOT}/sfml)

add_subdirectory(resources)
add_subdirectory(systems)
add_subdirectory(util)

set(SRC 
    ${SRC}
    PARENT_SCOPE)

# Build demo projects
if (${ECSTASY_INTEGRATIONS_SFML_BUILD_DEMO})
    add_subdirectory(demo)
endif()