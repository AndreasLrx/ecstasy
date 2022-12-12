set(INCROOT ${INCROOT}/toml)
set(SRCROOT ${SRCROOT}/toml)

set(SRC 
    ${SRC}
    ${SRCROOT}/TomlArrayNode.cpp
    ${INCROOT}/TomlArrayNode.hpp
    ${SRCROOT}/TomlConversion.cpp
    ${INCROOT}/TomlConversion.hpp
    ${INCROOT}/TomlNode.hpp
    PARENT_SCOPE)
