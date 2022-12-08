set(INCROOT ${INCROOT}/serialization)
set(SRCROOT ${SRCROOT}/serialization)

set(SRC 
    ${SRC}
    ${INCROOT}/foreach.hpp
    ${INCROOT}/IArrayNode.hpp
    ${INCROOT}/INode.hpp
    ${INCROOT}/IObjectNode.hpp
    ${INCROOT}/SerializableEnum.hpp
    ${INCROOT}/Serializer.hpp
    PARENT_SCOPE)
