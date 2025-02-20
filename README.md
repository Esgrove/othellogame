# othellogame

A simple [Othello (Reversi)](https://en.wikipedia.org/wiki/Reversi) CLI game implementation in different programming languages.

I originally did this in Python many years ago for my university basic programming course assignment,
and then a few years later re-implemented it in C++ for learning purposes.
Since then, I figured that this could actually be a useful personal project to:

1. try out and learn new languages by re-implementing it,
2. compare different languages easily with each other, and
3. be a practical syntax, formatting, and tooling reference for each language.

I have cleaned up and improved both my old implementations in Python and C++,
the two languages I have been using the most.
For each language, I try to make the program structure and code match as close as possible,
while also trying to follow the best practices and guidelines for each language.

While the program is quite simple,
it also covers pretty well most of the functionality and tooling one would commonly use in a programming language.
It's big enough to get a good feel for a language,
yet small enough that it could be done in a weekend even when starting with a new language.
It is also easy to check that the code is working correctly,
especially since it is possible to compare side-by-side with the other implementations.

So far, I have added _C#_, _Swift_, _Rust_, _Go_, and _Kotlin_ implementations.
I might still do a few other (modern) languages that interest me and/or have some hype around them,
such as _[Zig](https://ziglang.org/)_, _[Mojo](https://www.modular.com/max/mojo)_, and _[Ruby](https://www.ruby-lang.org/)_.

In the meantime, there's probably still room to improve in the current implementations,
especially in the languages that I haven't been actively using so much.

## Program structure

All implementations follow this overall architecture:

- _main_: Command line argument handling and entry point to initialize the game.
- _othello_: The main gameplay loop.
- _board_: Game board state and logic.
- _player_: Player object used for making moves and storing player state.
- _models_: Shared classes and enums.
- _settings_: Game and player settings.
- _utils_: Helper functions.
- _colorprint_: Helper methods for terminal color printing and string formatting (if needed).

## Languages

Listed in order of implementation from oldest to newest.

### Python

Python 3.11+.
Uses the [colorama](https://pypi.org/project/colorama/) package for colored text in the terminal.
Formatting follows [PEP8](https://www.python.org/dev/peps/pep-0008/) except for the outdated max line length of 79.
Uses [type hints](https://docs.python.org/3/library/typing.html#module-typing) heavily.
Dependencies are handled by [Poetry](https://github.com/python-poetry/poetry).
Formatting and linting with [ruff](https://github.com/astral-sh/ruff).

### C++

C++23.
Uses [CMake](https://cmake.org/) as the build system,
and [fmt](https://github.com/fmtlib/fmt) library for sensible string formatting and colored text.
Follows the [ISO](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rl-camel) recommended naming style (_snake_case_).
Code formatting is handled by [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html).

### C\#

.NET 9 and C# 11.
Uses [Pastel](https://github.com/silkfire/Pastel) for colored text in the terminal.
Follows the [C# style guide](https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/coding-style/coding-conventions) (_PascalCase_),
but with line length limit of 100.
Code formatting with [CSharpier](https://github.com/belav/csharpier).

### Swift

Swift 6.
Uses [ColorizeSwift](https://github.com/mtynior/ColorizeSwift) for colored text in the terminal.
Formatting with [swiftformat](https://github.com/nicklockwood/SwiftFormat).
Project is handled by the [Swift Package Manager](https://www.swift.org/package-manager/),
meaning there is no Xcode project and it works on all platforms.

### Rust

Rust 2021 edition.
Uses the [colored](https://crates.io/crates/colored) crate for colored text in the terminal.

### Go

Go 1.21+.
Uses [Aurora](https://github.com/logrusorgru/aurora) for colored text.

### Kotlin

Kotlin 1.9+ and [Gradle](https://gradle.org/kotlin/).

## TODO

- Write some thoughts on the languages and their differences
- Add unit tests for each language
