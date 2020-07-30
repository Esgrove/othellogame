cmake_minimum_required(VERSION 3.14)
project(othello_cpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    src
)

add_executable(
    othello_cpp
    src/board.cpp
    src/board.hpp
    src/othello.cpp
    src/othello.hpp
    src/player.cpp
    src/player.hpp
    src/util.cpp
    src/util.hpp
    src/colorprint.hpp
)

find_package(fmt 7 REQUIRED)
target_link_libraries(othello_cpp fmt::fmt)

message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
