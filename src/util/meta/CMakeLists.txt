set(INCROOT ${INCROOT}/meta)
set(SRCROOT ${SRCROOT}/meta)

set(SRC 
    ${SRC}
    ${INCROOT}/add_optional.hpp
    ${INCROOT}/contains.hpp
    ${INCROOT}/index.hpp
    ${INCROOT}/outer_join.hpp
    ${INCROOT}/Traits.hpp
    ${INCROOT}/type_set_eq.hpp
    PARENT_SCOPE)
