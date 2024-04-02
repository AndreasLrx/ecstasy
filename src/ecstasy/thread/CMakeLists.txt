set(INCROOT ${INCROOT}/thread)
set(SRCROOT ${SRCROOT}/thread)

set(SRC
    ${SRC}
    ${INCROOT}/LockableView.hpp
    ${SRCROOT}/SharedRecursiveMutex.cpp
    ${INCROOT}/SharedRecursiveMutex.hpp
    PARENT_SCOPE
)
