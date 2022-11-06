set(INCROOT ${INCROOT}/event)
set(SRCROOT ${SRCROOT}/event)

add_subdirectory(events)
add_subdirectory(inputs)
add_subdirectory(listeners)

set(SRC 
    ${SRC}
    ${SRCROOT}/EventsManager.cpp
    ${INCROOT}/EventsManager.hpp
    PARENT_SCOPE)
