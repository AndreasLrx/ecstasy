set(INCROOT ${INCROOT}/ecstasy)
set(SRCROOT ${SRCROOT}/ecstasy)

add_subdirectory(integrations)

add_subdirectory(query)
add_subdirectory(registry)
add_subdirectory(resources)
add_subdirectory(storages)
add_subdirectory(system)
if (${ECSTASY_MULTI_THREAD})
    add_subdirectory(thread)
endif()

configure_file(${SRCROOT}/config.hpp.in ${SRCROOT}/config.hpp)

set(SRC 
    ${SRC}
    PARENT_SCOPE)
