set(INCROOT ${INCROOT}/listeners)
set(SRCROOT ${SRCROOT}/listeners)

set(SRC 
    ${SRC}
    ${INCROOT}/EventListeners.hpp
    ${INCROOT}/GamepadAxisListener.hpp
    ${INCROOT}/GamepadButtonListener.hpp
    ${INCROOT}/GamepadConnectedListener.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/KeyListener.hpp
    ${SRCROOT}/KeySequenceListener.cpp
    ${INCROOT}/KeySequenceListener.hpp
    ${INCROOT}/MouseButtonListener.hpp
    ${INCROOT}/MouseMoveListener.hpp
    ${INCROOT}/MouseWheelScrollListener.hpp
    ${INCROOT}/TextEnteredListener.hpp
    PARENT_SCOPE)
