set(INCROOT ${INCROOT}/meta)
set(SRCROOT ${SRCROOT}/meta)

set(SRC 
    ${SRC}
    ${INCROOT}/add_optional.hpp
    ${INCROOT}/apply.hpp
    ${INCROOT}/contains.hpp
    ${INCROOT}/filter.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/index.hpp
    ${INCROOT}/is_std_vector.hpp
    ${INCROOT}/is_type_bounded_array.hpp
    ${INCROOT}/outer_join.hpp
    ${INCROOT}/Traits.hpp
    ${INCROOT}/type_set_eq.hpp
    PARENT_SCOPE)
