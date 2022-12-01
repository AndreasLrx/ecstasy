set(INCROOT ${INCROOT}/util)
set(SRCROOT ${SRCROOT}/util)

add_subdirectory(meta)
add_subdirectory(serialization)

set(SRC 
    ${SRC}
    ${INCROOT}/Allocator.hpp
    ${INCROOT}/BitSet.hpp
    ${SRCROOT}/BitSet.cpp
    PARENT_SCOPE)
