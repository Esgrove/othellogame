# Othello Kotlin Native

Requires Gradle.
Uses Kotlin Native to produce a native executable.

Uses [Clikt](https://github.com/ajalt/clikt) for the CLI.

## Usage

```console
Usage: othello_kotlin [<options>] [<size>]

  A simple Othello CLI game implementation in Kotlin

Options:
  -a, --autoplay    Enable autoplay mode with computer control
  -c, --check       Autoplay and only print result
  -d, --default     Play with default settings
  -l, --log         Show game log at the end
  -n, --no-helpers  Hide disk placement hints
  -t, --test        Enable test mode with deterministic computer moves
  -v, --version     Print version and exit
  -h, --help        Show this message and exit

Arguments:
  <size>  Optional board size (4..10)
```

## Build and run

```shell
./gradlew nativeBinaries
```

With shell script:

```shell
./build.sh
```

## Unit tests

```shell
./gradlew nativeTest
```

## Code formatting

Using [ktlint](https://github.com/pinterest/ktlint):

```shell
brew install ktlint

ktlint --format
```
