set(INCROOT ${INCROOT}/listeners)
set(SRCROOT ${SRCROOT}/listeners)

set(SRC 
    ${SRC}
    ${INCROOT}/EventListeners.hpp
    ${INCROOT}/KeyListener.hpp
    ${INCROOT}/listeners.hpp
    ${INCROOT}/MouseButtonListener.hpp
    ${INCROOT}/MouseMoveListener.hpp
    ${INCROOT}/MouseWheelScrollListener.hpp
    ${INCROOT}/TextEnteredListener.hpp
    PARENT_SCOPE)
