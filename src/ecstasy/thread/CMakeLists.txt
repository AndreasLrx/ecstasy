set(INCROOT ${INCROOT}/thread)
set(SRCROOT ${SRCROOT}/thread)

set(SRC
    ${SRC}
    ${INCROOT}/SharedObjectView.hpp
    ${SRCROOT}/SharedRecursiveMutex.cpp
    ${INCROOT}/SharedRecursiveMutex.hpp
    PARENT_SCOPE
)
