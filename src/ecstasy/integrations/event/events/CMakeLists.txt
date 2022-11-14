set(INCROOT ${INCROOT}/events)
set(SRCROOT ${SRCROOT}/events)

set(SRC 
    ${SRC}
    ${INCROOT}/Event.hpp
    ${INCROOT}/MouseButtonEvent.hpp
    ${INCROOT}/MouseWheelScrollEvent.hpp
    PARENT_SCOPE)
