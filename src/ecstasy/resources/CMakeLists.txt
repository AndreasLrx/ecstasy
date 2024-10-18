set(INCROOT ${INCROOT}/resources)
set(SRCROOT ${SRCROOT}/resources)

add_subdirectory(entity)

set(SRC
    ${SRC}
    ${INCROOT}/include.hpp
    ${INCROOT}/ObjectWrapper.hpp
    ${INCROOT}/IResource.hpp
    PARENT_SCOPE
)
