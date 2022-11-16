# Platform-specific compilation options

macro(ecstasy_set_compiler_flags target)
    if(COMPILER_TYPE MATCHES "msvc")
        message(STATUS "Enabling MSVC-specific options")

        target_compile_options(${target} PRIVATE 
            "/W4"
            "$<$<AND:$<CONFIG:RELEASE>,$<NOT:${BUILDING_TESTS}>>:/WX>"
        )
    elseif(COMPILER_TYPE MATCHES "gcc")
        message(STATUS "Enabling GCC-specific options")

        target_compile_options(${target} PRIVATE 
            "-Wall" "-Wextra" "-Wshadow" "-pedantic" "-Wpointer-arith"
            "-Wsuggest-override" "-Wconversion"
            "-Wno-unknown-pragmas"
            "$<$<AND:$<CONFIG:RELEASE>,$<NOT:${BUILDING_TESTS}>>:-O3;-Werror>"
            "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
        )

        if(ENABLE_TEST_COVERAGE)
            add_compile_options("--coverage" "-fprofile-arcs" "-ftest-coverage")
            add_link_options("--coverage" "-fprofile-arcs" "-ftest-coverage")
        endif()
    elseif(COMPILER_TYPE MATCHES "clang")
        message(STATUS "Enabling Clang-specific options")

        target_compile_options(${target} PRIVATE 
            "-Wall" "-Wextra" "-Wshadow" "-pedantic" "-Wpointer-arith"
            "-Wsuggest-override" "-Wconversion"
            "-Wno-unknown-pragmas"
            "$<$<AND:$<CONFIG:RELEASE>,$<NOT:${BUILDING_TESTS}>>:-O3;-Werror>"
            "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
        )
    endif()
endmacro()
