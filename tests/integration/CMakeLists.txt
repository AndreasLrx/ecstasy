set(SRCROOT ${SRCROOT}/integration)

if (${ECSTASY_INTEGRATIONS_EVENT})
    add_subdirectory(event)
endif()

if (${ECSTASY_INTEGRATIONS_USER_ACTION})
    add_subdirectory(user_action)
endif()
