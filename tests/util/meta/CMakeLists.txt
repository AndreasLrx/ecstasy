set(SRCROOT ${SRCROOT}/meta)

set(SRC
    ${SRC}
    ${SRCROOT}/tests_add_optional.cpp
    ${SRCROOT}/tests_apply.cpp
    ${SRCROOT}/tests_contains.cpp
    ${SRCROOT}/tests_filter.cpp
    ${SRCROOT}/tests_outer_join.cpp
    ${SRCROOT}/tests_type_set_eq.cpp
    PARENT_SCOPE
)
