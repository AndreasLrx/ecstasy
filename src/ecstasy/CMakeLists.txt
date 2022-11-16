set(INCROOT ${INCROOT}/ecstasy)
set(SRCROOT ${SRCROOT}/ecstasy)

add_subdirectory(integrations)

add_subdirectory(query)
add_subdirectory(registry)
add_subdirectory(resources)
add_subdirectory(storages)
add_subdirectory(system)

set(SRC 
    ${SRC}
    PARENT_SCOPE)

set(SYSTEM_INCDIRS 
    ${SYSTEM_INCDIRS} 
    PARENT_SCOPE)