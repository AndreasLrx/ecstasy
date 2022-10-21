set(INCROOT ${INCROOT}/registry)
set(SRCROOT ${SRCROOT}/registry)

set(SRC
    ${SRC}
    ${SRCROOT}/DeletionStack.cpp
    ${INCROOT}/DeletionStack.hpp
    ${SRCROOT}/Registry.cpp
    ${INCROOT}/Registry.hpp
    PARENT_SCOPE
)
