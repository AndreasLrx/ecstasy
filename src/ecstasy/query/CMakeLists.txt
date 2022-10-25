set(INCROOT ${INCROOT}/query)
set(SRCROOT ${SRCROOT}/query)

add_subdirectory(concepts)

set(SRC
    ${SRC}
    ${INCROOT}/ComplexQuery.hpp
    ${INCROOT}/Modifier.hpp
    ${INCROOT}/ModifiersList.hpp
    ${INCROOT}/NotModifier.hpp
    ${INCROOT}/Query.hpp
    PARENT_SCOPE
)
