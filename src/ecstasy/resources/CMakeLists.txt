set(INCROOT ${INCROOT}/resources)
set(SRCROOT ${SRCROOT}/resources)

add_subdirectory(entity)

set(SRC
    ${SRC}
    ${INCROOT}/ObjectWrapper.hpp
    ${INCROOT}/Resource.hpp
    ${INCROOT}/resources.hpp
    PARENT_SCOPE
)