set(INCROOT ${INCROOT}/resources)
set(SRCROOT ${SRCROOT}/resources)

add_subdirectory(entity)

set(SRC
    ${SRC}
    ${SRCROOT}/Resource.hpp
    PARENT_SCOPE
)