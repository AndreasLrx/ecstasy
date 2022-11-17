set(INCROOT ${INCROOT}/events)
set(SRCROOT ${SRCROOT}/events)

set(SRC 
    ${SRC}
    ${INCROOT}/Event.hpp
    ${INCROOT}/KeyEvent.hpp
    ${INCROOT}/KeyPressedEvent.hpp
    ${INCROOT}/KeyReleasedEvent.hpp
    ${INCROOT}/MouseButtonEvent.hpp
    ${INCROOT}/MouseButtonPressedEvent.hpp
    ${INCROOT}/MouseButtonReleasedEvent.hpp
    ${INCROOT}/MouseMoveEvent.hpp
    ${INCROOT}/MouseWheelScrollEvent.hpp
    PARENT_SCOPE)
