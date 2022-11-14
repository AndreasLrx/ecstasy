set(INCROOT ${INCROOT}/listeners)
set(SRCROOT ${SRCROOT}/listeners)

set(SRC 
    ${SRC}
    ${INCROOT}/MouseButtonListener.hpp
    ${INCROOT}/MouseMoveListener.hpp
    ${INCROOT}/MouseWheelScrollListener.hpp
    PARENT_SCOPE)
