set(INCROOT ${INCROOT}/concepts)
set(SRCROOT ${SRCROOT}/concepts)

set(SRC
    ${SRC}
    ${INCROOT}/add_optional.hpp
    ${INCROOT}/GetMissingTypes.hpp
    ${INCROOT}/Queryable.hpp
    ${INCROOT}/QueryableNeedAdjust.hpp
    ${INCROOT}/util.hpp
    PARENT_SCOPE
)
