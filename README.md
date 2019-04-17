# othellogame

A simple [Othello / Reversi](https://en.wikipedia.org/wiki/Reversi) command line game implementation in different programming languages.

I originally did this in Python for a programming course, and then a few years later redid the thing in C++ for fun. Now I though that this could actually be a nice way to compare languages directly and to illustrate the differences in syntax and usage. Also, these work well as a source code formatting reference. I cleaned up both of my old implementations in Python and C++, and refactored them to match eatch other as closely as is reasonable. The program structure and all the functions are basically identical, so it is very easy to compare them. 

I started working on a Rust version of the same thing as a learning excercise, and I'll probably do a few other languages, such as C, C#, Swift, Kotlin etc... in the future as well for learning and comparison purposes. One of the interesting things to compare will be the different level and type of OOP support in each language, and how that changes the implementation and program structure. For example Rust does OOP a bit differently, and of course C does not have classes at all out of the box.

### Program structure

_main_: just the main game loop
_othello_: the game logic implementation and game board
_player_: player object used for playing one move and storing player information
_util_: helper structs and functions for printing and formatting

### Python

Python 3 only. Uses the [colorama](https://pypi.org/project/colorama/) package as an extra for colored text in the terminal.

##### Files

* main.py
* othello.py
* player.py
* util.py
* color.py

### C++

Primarily done in VS2017 on Win10, but also includes a makefile for mac/linux and checked to work on my Macbook Pro under Mojave. 

##### Files

* main.cpp
* othello.hpp & othello.cpp
* player.hpp & player.cpp
* util.hpp & util.cpp