set(INCROOT ${INCROOT}/system)
set(SRCROOT ${SRCROOT}/system)

set(SRC
    ${SRC}
    ${INCROOT}/Pipeline.hpp
    ${SRCROOT}/Pipeline.cpp
    ${INCROOT}/ISystem.hpp
    ${INCROOT}/Timer.hpp
    ${SRCROOT}/Timer.cpp
    PARENT_SCOPE
)
