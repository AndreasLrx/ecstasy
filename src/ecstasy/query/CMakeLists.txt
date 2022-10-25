set(INCROOT ${INCROOT}/query)
set(SRCROOT ${SRCROOT}/query)

add_subdirectory(concepts)

set(SRC
    ${SRC}
    ${INCROOT}/Modifier.hpp
    ${INCROOT}/ModifiersList.hpp
    ${INCROOT}/NotModifier.hpp
    ${INCROOT}/Query.hpp
    ${INCROOT}/Select.hpp
    PARENT_SCOPE
)
