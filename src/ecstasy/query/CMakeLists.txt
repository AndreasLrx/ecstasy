set(INCROOT ${INCROOT}/query)
set(SRCROOT ${SRCROOT}/query)

add_subdirectory(concepts)
add_subdirectory(conditions)
add_subdirectory(modifiers)

set(SRC
    ${SRC}
    ${INCROOT}/include.hpp
    ${INCROOT}/Query.hpp
    ${INCROOT}/Select.hpp
    PARENT_SCOPE
)
