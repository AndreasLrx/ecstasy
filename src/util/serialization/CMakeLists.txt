set(INCROOT ${INCROOT}/serialization)
set(SRCROOT ${SRCROOT}/serialization)

set(SRC 
    ${SRC}
    ${INCROOT}/foreach.hpp
    ${INCROOT}/SerializableEnum.hpp
    ${INCROOT}/Serializer.hpp
    PARENT_SCOPE)
