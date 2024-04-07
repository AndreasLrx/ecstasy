set(INCROOT ${INCROOT}/concepts)
set(SRCROOT ${SRCROOT}/concepts)

set(SRC
    ${SRC}
    ${INCROOT}/ComponentType.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/modifier_allocator_size.hpp
    ${INCROOT}/QueryableType.hpp
    ${INCROOT}/RegistryModifier.hpp
    PARENT_SCOPE
)
