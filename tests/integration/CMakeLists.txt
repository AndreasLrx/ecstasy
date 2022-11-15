set(SRCROOT ${SRCROOT}/integration)

if (${ECSTASY_INTEGRATIONS_EVENT})
    add_subdirectory(event)
endif()
