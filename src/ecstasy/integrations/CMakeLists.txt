set(INCROOT ${INCROOT}/integrations)
set(SRCROOT ${SRCROOT}/integrations)

if (${ECSTASY_INTEGRATIONS_EVENT})
    add_subdirectory(event)
endif()

set(SRC 
    ${SRC}
    PARENT_SCOPE)
