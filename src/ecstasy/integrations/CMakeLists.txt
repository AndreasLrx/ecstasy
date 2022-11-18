set(INCROOT ${INCROOT}/integrations)
set(SRCROOT ${SRCROOT}/integrations)

if (${ECSTASY_INTEGRATIONS_EVENT})
    add_subdirectory(event)
endif()

if (${ECSTASY_INTEGRATIONS_SFML})
    add_subdirectory(sfml)
endif()

set(SRC 
    ${SRC}
    ${INCROOT}/include.hpp
    PARENT_SCOPE)
