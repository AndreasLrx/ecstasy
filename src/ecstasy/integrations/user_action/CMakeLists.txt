set(INCROOT ${INCROOT}/user_action)
set(SRCROOT ${SRCROOT}/user_action)

set(SRC 
    ${SRC}
    ${INCROOT}/Action.hpp
    ${INCROOT}/ActionBinding.hpp
    ${SRCROOT}/ActionBindings.cpp
    ${INCROOT}/ActionBindings.hpp
    ${INCROOT}/ActionListener.hpp
    ${SRCROOT}/UserProfile.cpp
    ${INCROOT}/UserProfile.hpp
    ${SRCROOT}/Users.cpp
    ${INCROOT}/Users.hpp
    PARENT_SCOPE)
