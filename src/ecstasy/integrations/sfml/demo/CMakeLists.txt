set(INCROOT ${INCROOT}/demo)
set(SRCROOT ${SRCROOT}/demo)

set(SFML_DEMO_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/demo/sfml")
file(MAKE_DIRECTORY ${SFML_DEMO_OUTPUT_DIRECTORY})

add_subdirectory(01_basic_window)
