set(INCROOT ${INCROOT}/serialization)
set(SRCROOT ${SRCROOT}/serialization)

set(SRC 
    ${SRC}
    ${SRCROOT}/ANode.cpp
    ${INCROOT}/ANode.hpp
    ${INCROOT}/foreach.hpp
    ${INCROOT}/IArrayNode.hpp
    ${INCROOT}/INode.hpp
    ${INCROOT}/IObjectNode.hpp
    ${INCROOT}/SerializableEnum.hpp
    ${INCROOT}/Serializer.hpp
    PARENT_SCOPE)
