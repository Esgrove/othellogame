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

add_executable(othello_tests)

target_sources(othello_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/types.cpp
  ${CMAKE_SOURCE_DIR}/src/utils.cpp
  test_types.cpp
  test_utils.cpp
)

target_include_directories(othello_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

get_property(VERSION_INFO_DEFINITIONS GLOBAL PROPERTY GLOBAL_COMPILE_DEFINITIONS)

target_compile_definitions(othello_tests PRIVATE ${VERSION_INFO_DEFINITIONS})

target_link_libraries(othello_tests
    gtest_main
    fmt::fmt
    OpenSSL::Crypto
)

include(GoogleTest)
gtest_discover_tests(othello_tests)
