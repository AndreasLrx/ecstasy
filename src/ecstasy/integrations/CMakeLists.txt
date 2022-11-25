set(INCROOT ${INCROOT}/integrations)
set(SRCROOT ${SRCROOT}/integrations)

if (${ECSTASY_INTEGRATIONS_EVENT})
    add_subdirectory(event)
endif()

if (${ECSTASY_INTEGRATIONS_SFML})
    add_subdirectory(sfml)
endif()

if (${ECSTASY_INTEGRATIONS_USER_ACTION})
    add_subdirectory(user_action)
endif()

set(SRC 
    ${SRC}
    ${INCROOT}/include.hpp
    PARENT_SCOPE)
