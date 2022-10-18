set(INCROOT ${INCROOT}/resource)
set(SRCROOT ${SRCROOT}/resource)

set(SRC
    ${SRC}
    ${SRCROOT}/Resource.hpp
    PARENT_SCOPE
)

add_subdirectory(entity)
