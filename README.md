# othellogame

A simple [Othello (Reversi)](https://en.wikipedia.org/wiki/Reversi) command line game implementation in different programming languages.

I originally did this in Python many years ago for a university programming course assignment,
and then a few years later redid it in C++ for learning purposes.
Since then, I figured that this could actually be a useful project to:

1. learn new languages by re-implementing it,
2. compare different languages easily with each other, and
3. be a practical syntax and formatting reference.

I have since cleaned up and improved both my old implementations in Python and C++,
the two languages I have been using the most.
For each language, I try to make the program structure and all the functions match as close as possible,
while also trying to follow the best practices and guidelines for each language.

This makes it very easy to compare the implementations to each other.
While the project is quite simple,
it also covers pretty well most of the basic functionality and tooling one would commonly use in a programming language.
It's big enough to get a good feel for a language,
yet small enough that it could be done in a weekend even when starting with a new language.
It is also quite easy to check that the code is working correctly,
especially since it is possible to compare side-by-side with the other implementations.

So far, I have added *C#*, *Swift*, *Rust*, and *Go* implementations.
I might still do a few other (modern) languages that interest me and/or have some hype around them,
such as *Kotlin*, *Ruby*, and *Zig*.

In the meantime, there's probably still room to improve in the current implementations,
especially in the languages that I haven't been actively using.

## Program structure

All implementations follow this overall architecture:

* *main*: command line argument handling and entry point to initialize the game.
* *othello*: The main gameplay loop.
* *board*: Game board state and logic.
* *player*: Player object used for making moves and storing player information.
* *utils*: Helper classes, enums and functions.
* *colorprint*: Helper methods for color printing and string formatting (if needed).

## Languages

### Python

Python 3.10+.
Uses the [colorama](https://pypi.org/project/colorama/) package for colored text in the terminal.
Formatting follows [PEP8](https://www.python.org/dev/peps/pep-0008/) except for the outdated max line length of 79.
Uses [type hints](https://docs.python.org/3/library/typing.html#module-typing) heavily.
Dependencies are handled by [Poetry](https://github.com/python-poetry/poetry).
Formatting with [Black](https://github.com/psf/black) and [isort](https://github.com/PyCQA/isort),
linting with [ruff](https://github.com/astral-sh/ruff).

* othello.py
* board.py
* player.py
* utils.py
* colorprint.py

### C++

C++20. Uses [CMake](https://cmake.org/) as the build system,
and [fmt](https://github.com/fmtlib/fmt) library for sensible string formatting and colored text (stringstreams :unamused:).
Follows the [ISO](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rl-camel) recommended naming style (*snake_case*),
which sadly very few C++ codebases seem to be using (in my experience).
Code formatting is handled by [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html).

* main.cpp
* othello.hpp & othello.cpp
* board.hpp & board.cpp
* player.hpp & player.cpp
* utils.hpp
* colorprint.hpp
* version.hpp

### C\#

.NET 7 and C# 8.
Uses [Pastel](https://github.com/silkfire/Pastel) for colored text in the terminal.
Follows the C# style guide (*PascalCase*).

* Othello.cs
* Board.cs
* Player.cs
* Util.cs (*'Utils.cs'* does not work with VS syntax highlight for me :man_shrugging:)
* ColorPrint.cs

### Swift

Swift 5.
Uses [ColorizeSwift](https://github.com/mtynior/ColorizeSwift) for colored text in the terminal.
Formatting with [swiftformat](https://github.com/nicklockwood/SwiftFormat).
Project is handled by the [Swift Package Manager](https://www.swift.org/package-manager/),
so there is no Xcode project.

* main.swift
* othello.swift
* board.swift
* player.swift
* utils.swift
* colorprint.swift
* version.h

### Rust

Rust 2021 edition.
Uses the [colored](https://crates.io/crates/colored) crate for colored text in the terminal.

* main.rs
* othello.rs
* board.rs
* player.rs
* utils.rs
* colorprint.rs

### Go

Go 1.21+.
Uses [Aurora](https://github.com/logrusorgru/aurora) for colored text.

* main.go
* othello/othello.go
* othello/board.go
* othello/player.go
* othello/utils.go
* othello/colorprint.go

## TODO

* Add unit tests for each language
* Unify naming and comments across the different implementations
* Make a NumPy version of Python implementation (not really needed but why not)
* Write some thoughts on the languages and their differences
