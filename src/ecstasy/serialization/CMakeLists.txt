set(INCROOT ${INCROOT}/serialization)
set(SRCROOT ${SRCROOT}/serialization)

set(SRC
    ${SRC}
    ${INCROOT}/ComponentSerializer.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/is_serializable.hpp
    ${INCROOT}/ISerializer.hpp
    ${INCROOT}/RawSerializer.hpp
    ${INCROOT}/Serializer.hpp
    PARENT_SCOPE
)
