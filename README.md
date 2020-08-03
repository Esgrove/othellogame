# othellogame

A simple [Othello / Reversi](https://en.wikipedia.org/wiki/Reversi) command line game implementation in different programming languages.

I originally did this in Python many years ago for a programming course, and then a few years later redid it in C++ for fun (and learning purposes). Since then I figured that this could actually be an useful project to: 
1. learn new languages by re-implenting it,
2. compare different languages directly with each other, and
3. be a practical code formatting and syntax reference.

I have since cleaned up and improved both my old implementations in Python and C++, the two languages I have been using the most. For each language, I try to make the program structure and all the functions match as close as possible, while also trying to follow the best practices and guidelines for each language. This makes it very easy to compare the implementations to each other. While the project is quite simple, it also covers pretty well most of the basic functionality and tooling one would commonly use in a programming language. It's big enough to get a good feel for a language, yet small enough that it could be done even in a weekend when starting with a new language. It is also very easy to see that the code is working correctly, especially since it is possible to compare side-by-side with the older implementations. 

Since then I have added a *C#* implementation, and started working on a *Rust* version as a learning exercise. I'll probably do a few other (modern) languages that interest me, such as *Swift*, *Go* and *Kotlin*. It would also be interesting to try to make a functional / non-OOP version at some point...

## Program structure

_othello_: The main gameplay loop.  
_board_: Game board state and logic.  
_player_: Player object used for making moves and storing player information.  
_utils_: Helper classes and methods.  
_colorprint_: Helper methods for color and style formatting.

## Languages

### Python
Python 3.6+. Uses the [colorama](https://pypi.org/project/colorama/) package for colored text in the terminal. Formatting follows [PEP8](https://www.python.org/dev/peps/pep-0008/) except for the outdated max line length of 80. Uses [type hints](https://docs.python.org/3/library/typing.html#module-typing) heavily.
* othello.py
* board.py
* player.py
* utils.py
* colorprint.py

### C++
C++17. Uses [Cmake](https://cmake.org/) as the build system. Uses the [fmt](https://github.com/fmtlib/fmt) library for sensible string formatting and colored text (stringstreams :unamused:). Follows the [ISO](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-naming) recommended naming style (snake_case), which sadly very few C++ codebases seem to be using (in my experience).
* othello.hpp & othello.cpp
* board.hpp & board.cpp
* player.hpp & player.cpp
* utils.hpp
* colorprint.hpp

### C#
Uses [Pastel](https://github.com/silkfire/Pastel) for colored text in the terminal. Follows the C# style guide (PascalCase) otherwise, except for always putting opening brackets on their own line (all that wasted whitespace :grimacing:).
* Othello.cs
* Board.cs
* Player.cs
* Utils.cs
* ColorPrint.cs

### Rust

*In progress...*


## TODO

* Add unit tests for each language
* Make a NumPy version of Python implementation
