# Copilot Instructions for Othellogame

## Project Overview

This is a polyglot implementation of Othello (Reversi) - a CLI game implemented in multiple programming languages for learning and comparison purposes. Each implementation follows the same architecture and logic while adhering to language-specific best practices.

## Project Goals

1. Learn and compare different programming languages
2. Maintain consistent architecture across all implementations
3. Follow best practices and idiomatic code for each language
4. Provide a practical reference for syntax, formatting, and tooling

## Architecture

All language implementations follow this consistent structure:

- **main**: Command line argument handling and game initialization
- **othello**: Main gameplay loop
- **board**: Game board state and logic
- **player**: Player object for moves and state management
- **models**: Shared classes and enums
- **settings**: Game and player configuration
- **utils**: Helper functions
- **colorprint**: Terminal color printing and string formatting helpers

## Language-Specific Guidelines

### Python (3.11+)
- Use type hints extensively
- Follow PEP8 style (max line length: 100 chars, not 79)
- Use Poetry for dependency management
- Format with ruff
- Lint with ruff
- Use colorama for terminal colors
- Run tests with pytest

### C++ (C++23)
- Use CMake as build system
- Follow ISO recommended naming style (snake_case)
- Use fmt library for formatting and colors
- Format with ClangFormat
- Lint with clang-tidy
- Max line length: 100 chars

### C# (.NET 9, C# 11)
- Follow C# style guide (PascalCase)
- Max line length: 100 chars
- Use Pastel for terminal colors
- Format with CSharpier

### Swift (Swift 6)
- Use Swift Package Manager (no Xcode project)
- Format with swiftformat
- Use ColorizeSwift for terminal colors
- Must work cross-platform

### Rust (2024 edition)
- Use cargo for build and dependency management
- Format with rustfmt
- Lint with clippy
- Use colored crate for terminal colors

### Go (1.21+)
- Use go fmt for formatting
- Use gofmt/golint for linting
- Use Aurora for terminal colors
- Follow standard Go project structure

### Kotlin (1.9+)
- Use Gradle for build management
- Follow Kotlin coding conventions

### Java (Java 21)
- Use Gradle for build management
- Follow standard Java conventions

### TypeScript
- Follow TypeScript best practices
- Use appropriate type definitions

### Zig
- Follow Zig conventions
- Use zig build system

## Code Consistency Rules

1. **Architecture Consistency**: When modifying one language implementation, consider if similar changes are needed in other languages to maintain feature parity
2. **Minimal Changes**: Keep implementations as similar as possible across languages while respecting language idioms
3. **File Organization**: Match file and module structure across all implementations
4. **Naming Conventions**: Use the same conceptual names for classes, methods, and variables (adapted to language naming conventions)

## Development Workflow

1. **Before Making Changes**:
   - Check if similar functionality exists in other language implementations
   - Review language-specific formatting and linting rules
   - Understand the existing test infrastructure

2. **Testing**:
   - Each language has its own test directory
   - Run language-specific tests before committing
   - Maintain test coverage where it exists
   - Tests should validate game logic consistency

3. **Formatting and Linting**:
   - Always run the language's formatter before committing
   - Fix linting issues for the specific language
   - Follow the .editorconfig settings (4 spaces, UTF-8, LF line endings)

4. **Documentation**:
   - Update README.md if adding new languages or major features
   - Update language-specific README files for implementation details
   - Keep comments concise and explain "why" not "what"

## CI/CD

- Each language has its own GitHub Actions workflow in `.github/workflows/`
- Workflows run on pushes to main and pull requests
- Workflows include formatting checks, linting, and testing
- All checks must pass before merging

## Common Pitfalls to Avoid

1. Don't mix language conventions (e.g., don't use snake_case in C# or PascalCase in Python)
2. Don't add dependencies without checking if they're necessary
3. Don't modify unrelated language implementations when fixing a single language
4. Don't remove or disable existing tests
5. Don't commit build artifacts or language-specific temporary files (check .gitignore)

## Dependencies and Security

- When adding or updating dependencies, ensure they're appropriate for the language
- Check for security vulnerabilities in dependencies
- Use version pinning where appropriate for each language's package manager

## Making Changes

When asked to implement a feature or fix a bug:

1. Identify which language(s) are affected
2. Check if the same change should apply to other implementations
3. Make minimal, focused changes
4. Test in the specific language environment
5. Run formatting and linting tools
6. Verify that CI checks will pass

## Editor Configuration

The repository uses .editorconfig with these settings:
- Charset: UTF-8
- Line endings: LF
- Indent: 4 spaces (tab for Go)
- Insert final newline: true
- Trim trailing whitespace: true
- Max line length: 100 chars
