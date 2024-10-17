set(INCROOT ${INCROOT}/concepts)
set(SRCROOT ${SRCROOT}/concepts)

set(SRC
    ${SRC}
    ${INCROOT}/component_type.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/modifier_allocator_size.hpp
    ${INCROOT}/queryable_type.hpp
    ${INCROOT}/RegistryModifier.hpp
    PARENT_SCOPE
)
