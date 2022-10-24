set(INCROOT ${INCROOT}/query)
set(SRCROOT ${SRCROOT}/query)

set(SRC
    ${SRC}
    ${INCROOT}/ComplexQuery.hpp
    ${INCROOT}/Query.hpp
    ${INCROOT}/QueryConcepts.hpp
    ${INCROOT}/QueryModifiers.hpp
    PARENT_SCOPE
)
