set(INCROOT ${INCROOT}/event)
set(SRCROOT ${SRCROOT}/event)

add_subdirectory(events)
add_subdirectory(inputs)

set(SRC 
    ${SRC}
    PARENT_SCOPE)
