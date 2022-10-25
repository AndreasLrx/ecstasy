set(INCROOT ${INCROOT}/registry)
set(SRCROOT ${SRCROOT}/registry)

add_subdirectory(concepts)
add_subdirectory(modifiers)

set(SRC
    ${SRC}
    ${SRCROOT}/DeletionStack.cpp
    ${INCROOT}/DeletionStack.hpp
    ${SRCROOT}/Registry.cpp
    ${INCROOT}/Registry.hpp
    PARENT_SCOPE
)
