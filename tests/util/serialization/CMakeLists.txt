set(SRCROOT ${SRCROOT}/serialization)

set(SRC)

add_subdirectory(toml)

add_ecstasy_test(
    NAME ecstasy_test_util_serialization
    DISPLAY_NAME "Ecstasy-Util-Serialization-Tests"
    SOURCES ${SRC}
    INCLUDE_DIRS ${TomlPlusPlus_IncDir}
)
