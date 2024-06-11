set(INCROOT ${INCROOT}/serialization)
set(SRCROOT ${SRCROOT}/serialization)

set(SRC
    ${SRC}
    ${INCROOT}/include.hpp
    ${INCROOT}/ISerializer.hpp
    ${INCROOT}/RawSerializer.hpp
    ${INCROOT}/Serializer.hpp
    PARENT_SCOPE
)
