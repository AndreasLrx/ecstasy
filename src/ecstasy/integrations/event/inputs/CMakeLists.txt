set(INCROOT ${INCROOT}/inputs)
set(SRCROOT ${SRCROOT}/inputs)

set(SRC 
    ${SRC}
    ${INCROOT}/inputs.hpp
    ${SRCROOT}/Keyboard.cpp
    ${INCROOT}/Keyboard.hpp
    ${INCROOT}/Mouse.hpp
    PARENT_SCOPE)
