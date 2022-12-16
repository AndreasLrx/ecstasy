set(INCROOT ${INCROOT}/serialization)
set(SRCROOT ${SRCROOT}/serialization)

if (${ECSTASY_SERIALIZER_TOML})
    add_subdirectory(toml)
endif()

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
