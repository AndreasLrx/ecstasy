set(INCROOT ${INCROOT}/inputs)
set(SRCROOT ${SRCROOT}/inputs)

set(SRC 
    ${SRC}
    ${SRCROOT}/Gamepad.cpp
    ${INCROOT}/Gamepad.hpp
    ${INCROOT}/Gamepads.hpp
    ${INCROOT}/include.hpp
    ${SRCROOT}/Keyboard.cpp
    ${INCROOT}/Keyboard.hpp
    ${SRCROOT}/Mouse.cpp
    ${INCROOT}/Mouse.hpp
    PARENT_SCOPE)
