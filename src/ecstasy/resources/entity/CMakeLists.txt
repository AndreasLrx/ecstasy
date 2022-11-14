set(INCROOT ${INCROOT}/entity)
set(SRCROOT ${SRCROOT}/entity)

set(SRC
    ${SRC}
    ${SRCROOT}/Entities.cpp
    ${INCROOT}/Entities.hpp
    ${INCROOT}/Entity.hpp
    ${INCROOT}/RegistryEntity.hpp
    PARENT_SCOPE
)

