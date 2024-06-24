set(INCROOT ${INCROOT}/concepts)
set(SRCROOT ${SRCROOT}/concepts)

set(SRC
    ${SRC}
    ${INCROOT}/has_extraction_operator.hpp
    ${INCROOT}/has_insertion_operator.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/is_serializer.hpp
    PARENT_SCOPE
)

