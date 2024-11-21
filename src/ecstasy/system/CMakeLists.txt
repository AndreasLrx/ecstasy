set(INCROOT ${INCROOT}/system)
set(SRCROOT ${SRCROOT}/system)

set(SRC
    ${SRC}
    ${INCROOT}/Pipeline.hpp
    ${SRCROOT}/Pipeline.cpp
    ${INCROOT}/ISystem.hpp
    PARENT_SCOPE
)
