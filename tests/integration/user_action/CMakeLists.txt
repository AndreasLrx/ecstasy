set(SRCROOT ${SRCROOT}/user_action)

set(SRC
    ${SRCROOT}/tests_ActionBinding.cpp
    ${SRCROOT}/tests_ActionBindings.cpp
)

add_ecstasy_test(
    NAME ecstasy_test_integration_user_action
    DISPLAY_NAME "Ecstasy-Integration-UserAction-Tests"
    SOURCES ${SRC}
    INCLUDE_DIRS ${TomlPlusPlus_IncDir}
)
