set(INCROOT ${INCROOT}/rtti)
set(SRCROOT ${SRCROOT}/rtti)

set(SRC
    ${SRC}
    ${SRCROOT}/AType.cpp
    ${INCROOT}/AType.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/Type.hpp
    ${SRCROOT}/TypeRegistry.cpp
    ${INCROOT}/TypeRegistry.hpp
    PARENT_SCOPE
)
