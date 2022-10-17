set(INCROOT ${INCROOT}/ecstasy)
set(SRCROOT ${SRCROOT}/ecstasy)


add_subdirectory(registry)
add_subdirectory(storage)
add_subdirectory(system)

set(SRC 
    ${SRC}
    PARENT_SCOPE)
