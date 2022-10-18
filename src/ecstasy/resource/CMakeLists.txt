set(INCROOT ${INCROOT}/resource)
set(SRCROOT ${SRCROOT}/resource)

add_subdirectory(entity)

set(SRC
    ${SRC}
    ${SRCROOT}/Resource.hpp
    PARENT_SCOPE
)