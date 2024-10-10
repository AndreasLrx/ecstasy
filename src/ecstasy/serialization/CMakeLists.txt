set(INCROOT ${INCROOT}/serialization)
set(SRCROOT ${SRCROOT}/serialization)

add_subdirectory(concepts)
add_subdirectory(traits)

if(${ECSTASY_SERIALIZER_JSON})
    set(SRC
        ${SRC}
        ${INCROOT}/JsonSerializer.hpp
        PARENT_SCOPE
    )
endif()


set(SRC
    ${SRC}
    ${INCROOT}/EntityComponentSerializer.hpp
    ${INCROOT}/IEntityComponentSerializer.hpp
    ${INCROOT}/include.hpp
    ${INCROOT}/ISerializer.hpp
    ${INCROOT}/RawSerializer.hpp
    ${INCROOT}/Serializer.hpp
    PARENT_SCOPE
)
