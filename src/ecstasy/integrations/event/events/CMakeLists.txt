set(INCROOT ${INCROOT}/events)
set(SRCROOT ${SRCROOT}/events)

set(SRC 
    ${SRC}
    ${INCROOT}/Event.hpp
    ${INCROOT}/MouseButtonPressedEvent.hpp
    ${INCROOT}/MouseButtonReleasedEvent.hpp
    PARENT_SCOPE)
