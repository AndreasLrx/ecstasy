set(INCROOT ${INCROOT}/modifiers)
set(SRCROOT ${SRCROOT}/modifiers)

set(SRC
    ${SRC}
    ${INCROOT}/And.hpp
    ${INCROOT}/Maybe.hpp
    ${INCROOT}/modifiers.hpp
    ${INCROOT}/Not.hpp
    ${INCROOT}/Or.hpp
    ${INCROOT}/Xor.hpp
    PARENT_SCOPE
)
