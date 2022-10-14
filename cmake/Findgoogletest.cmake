cmake_minimum_required(VERSION 3.14)

include(FetchContent)
    
# GoogleTest v1.11.0
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
    URL_HASH MD5=feb94215fd6d9eb3b5f14d06d97ece00
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
