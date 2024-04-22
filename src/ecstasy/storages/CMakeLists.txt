set(INCROOT ${INCROOT}/storages)
set(SRCROOT ${SRCROOT}/storages)

set(SRC
    ${SRC}
    ${INCROOT}/include.hpp
    ${INCROOT}/Instances.hpp
    ${INCROOT}/IStorage.hpp
    ${INCROOT}/MapStorage.hpp
    ${INCROOT}/MarkerStorage.hpp
    ${INCROOT}/StorageConcepts.hpp
    PARENT_SCOPE
)
