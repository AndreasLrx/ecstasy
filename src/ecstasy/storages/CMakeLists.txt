set(INCROOT ${INCROOT}/storages)
set(SRCROOT ${SRCROOT}/storages)

set(SRC
    ${SRC}
    ${INCROOT}/Instances.hpp
    ${INCROOT}/IStorage.hpp
    ${INCROOT}/MapStorage.hpp
    ${INCROOT}/StoragesConcepts.hpp
    ${INCROOT}/storages.hpp
    PARENT_SCOPE
)
