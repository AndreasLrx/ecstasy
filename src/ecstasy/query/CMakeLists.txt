set(INCROOT ${INCROOT}/query)
set(SRCROOT ${SRCROOT}/query)

add_subdirectory(concepts)
add_subdirectory(modifiers)

set(SRC
    ${SRC}
    ${INCROOT}/Query.hpp
    ${INCROOT}/Select.hpp
    PARENT_SCOPE
)
