# othellogame

A simple [Othello / Reversi](https://en.wikipedia.org/wiki/Reversi) command line game implementation in different programming languages.

I originally did this years ago in Python for a programming course, and then a few years later redid the thing in C++ for fun. Now I though that this could actually be a nice way to compare languages directly and to illustrate the differences in syntax and usage. Also, these work well as a code formatting reference. I cleaned up both of my old implementations in Python and C++, and refactored them to match eatch other as closely as is reasonable. The program structure and all the functions are basically identical, so it is very easy to compare them.

I started working on a Rust version of the same thing as a learning excercise, and I'll probably do a few other languages, such as C#, Swift, Kotlin etc... in the future as well for learning purposes.

### Program structure

_othello_: The main gameplay loop.

_board_: Game board storage and functions.

_player_: Player object used for making moves and storing player information.

_util_: Some helper utilities and functions for example for string formatting and printing.

### Python

Python 3.6+ only. Uses [colorama](https://pypi.org/project/colorama/) for colored text in the terminal.

##### Files

* main.py
* othello.py
* player.py
* util.py
* colorprint.py

### C++

C++17. Uses [termcolor](https://github.com/ikalnytskyi/termcolor) for colored text in the terminal.

##### Files

* main.cpp
* othello.hpp & othello.cpp
* player.hpp & player.cpp
* util.hpp & util.cpp
* colorprint.hpp & colorprint.cpp
