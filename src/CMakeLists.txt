set(SRCROOT ${PROJECT_SOURCE_DIR}/src)
set(INCROOT ${PROJECT_SOURCE_DIR}/src)

set(SRC)
set(INCDIRS ${INCROOT})

add_subdirectory(ecstasy)
add_subdirectory(util)


# Source groups are a feature in Visual Studio.
source_group("" FILES ${SRC})

set(TARGET_NAME ecstasy)
set(LIB_NAME ${TARGET_NAME})

# Take in account shared/static library build
if(${BUILD_SHARED_LIBS})
    add_library(${LIB_NAME} SHARED ${SRC})
else()
    add_library(${LIB_NAME} STATIC ${SRC})
endif()

ecstasy_set_compiler_flags(${LIB_NAME})
# adjust output suffix depending on output type
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX -d)
# set version and soversion of target
set_target_properties(${LIB_NAME} PROPERTIES SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR})
set_target_properties(${LIB_NAME} PROPERTIES VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Integration links
if (${ECSTASY_INTEGRATIONS_SFML})
    target_link_libraries_system(${LIB_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
endif()

target_include_directories(${LIB_NAME} PRIVATE ${INCDIRS})
target_include_directories(${LIB_NAME} PUBLIC ${INCROOT})

add_custom_command(TARGET ${LIB_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIB_NAME}> ${ECSTASY_OUTPUT_DIRECTORY})
