set(INCROOT ${INCROOT}/listeners)
set(SRCROOT ${SRCROOT}/listeners)

set(SRC 
    ${SRC}
    ${INCROOT}/KeyListener.hpp
    ${INCROOT}/MouseButtonListener.hpp
    ${INCROOT}/MouseMoveListener.hpp
    ${INCROOT}/MouseWheelScrollListener.hpp
    ${INCROOT}/TextEnteredListener.hpp
    PARENT_SCOPE)
