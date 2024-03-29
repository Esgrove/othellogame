# Enable CTest integration
enable_testing()

# https://github.com/google/googletest
message(STATUS "Fetching GoogleTest library")
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG main
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(othello_tests
    test_utils.cpp
    ../src/utils.cpp
)

target_link_libraries(othello_tests
    gtest_main
    fmt::fmt
    OpenSSL::Crypto
)

include(GoogleTest)
gtest_discover_tests(othello_tests)
