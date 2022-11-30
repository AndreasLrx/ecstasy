set(INCROOT ${INCROOT}/inputs)
set(SRCROOT ${SRCROOT}/inputs)

set(SRC 
    ${SRC}
    ${INCROOT}/Gamepad.hpp
    ${INCROOT}/Gamepads.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/Keyboard.hpp
    ${SRCROOT}/Mouse.cpp
    ${INCROOT}/Mouse.hpp
    PARENT_SCOPE)
