cmake_minimum_required(VERSION 3.14)
project(othello_cpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    src
    include
    include
)

add_executable(
    othello_cpp
    src/board.cpp
    src/board.hpp
    src/colorprint.hpp
    src/othello.cpp
    src/othello.hpp
    src/player.cpp
    src/player.hpp
    src/util.cpp
    src/util.hpp
)
