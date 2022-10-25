set(INCROOT ${INCROOT}/query)
set(SRCROOT ${SRCROOT}/query)

set(SRC
    ${SRC}
    ${INCROOT}/ComplexQuery.hpp
    ${INCROOT}/Modifier.hpp
    ${INCROOT}/ModifiersList.hpp
    ${INCROOT}/NotModifier.hpp
    ${INCROOT}/Query.hpp
    ${INCROOT}/QueryConcepts.hpp
    PARENT_SCOPE
)
