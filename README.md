# othellogame

A simple [Othello / Reversi](https://en.wikipedia.org/wiki/Reversi) command line game implementation in different programming languages.

I originally did this in Python many years ago for a programming course, and then a few years later redid it in C++ for fun (and learning purposes). Since then I figured that this could actually be an useful project to: 
1. learn new languages by re-implenting it,
2. compare different languages directly with each other, 
3. Exist as a practical code formatting and syntax reference.

I have since cleaned up and improved both my old implementations in Python and C++, the two languages I have been using the most. For each language, I try to make the program structure and all the functions match as close as possible, while also trying to follow the best practices and guidelines for each language. This makes it very easy to compare the implementations to each other. While the project is small and trivial-to-implement, it also covers pretty well all the basic functionality and tooling one would need from a programming language. It is also easy to check that the code is working correctly.

Since then I have added a C# implementation, and started working on a Rust version as a learning excercise. I'll probably do a few other (modern) languages that interest me, such as Swift and Kotlin. It would also be interesting to try to make a functional / non-OOP version at some point...

### Program structure

_othello_: The main gameplay loop.  
_board_: Game board state and functions.  
_player_: Player object used for making moves and storing player information.  
_util_: Helper classes and functions.
_colorprint_: Helpers / wrappers for color and style formatting.

### TODO

* Add unit tests for each language

### Languages

#### Python

Requires Python 3.6+. Uses [colorama](https://pypi.org/project/colorama/) for colored text in the terminal.  
* othello.py
* board.py
* player.py
* util.py
* colorprint.py

#### C++

C++17. [Cmake](https://cmake.org/) as build system. Uses [fmt](https://github.com/fmtlib/fmt) library for sensible string formatting and colored text (stringstreams :unamused:). Follows the [ISO](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-naming) recommended naming style (snake_case), which sadly very few C++ codebases seem to be using (in my experience).
* othello.hpp & othello.cpp
* board.hpp & board.cpp
* player.hpp & player.cpp
* util.hpp
* colorprint.hpp

#### C#

No external dependencies. Follows the C# style guide (Pascal case etc) otherwise, except for putting opening brackets on their own line (all that wasted whitespace :grimacing:)
* Othello.cs
* Board.cs
* Player.cs
* Utils.cs
* ColorPrint.cs

#### Rust

*In progress...*
