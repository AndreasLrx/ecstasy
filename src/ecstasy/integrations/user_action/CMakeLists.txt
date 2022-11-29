set(INCROOT ${INCROOT}/user_action)
set(SRCROOT ${SRCROOT}/user_action)

set(SRC 
    ${SRC}
    ${INCROOT}/Action.hpp
    ${SRCROOT}/ActionBinding.cpp
    ${INCROOT}/ActionBinding.hpp
    ${SRCROOT}/ActionBindings.cpp
    ${INCROOT}/ActionBindings.hpp
    ${SRCROOT}/UserProfile.cpp
    ${INCROOT}/UserProfile.hpp
    PARENT_SCOPE)
