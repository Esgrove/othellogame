# Othello Java

## Usage

```console
Usage: java -jar othello_java [<options>] [<size>]

A simple Othello CLI game implementation in Java

      [size]         Optional board size (4..10)
  -a, --autoplay     Enable autoplay mode with computer control
  -c, --check        Autoplay and only print result
  -d, --default      Play with default settings
  -h, --help         Show this help message and exit.
  -l, --log          Show game log at the end
  -n, --no-helpers   Hide disk placement hints
  -t, --test         Enable test mode with deterministic computer moves
  -V, --version      Print version information and exit.
```

## Run

```shell
./othello_java.sh
./othello_java.sh --help
./othello_java.sh --version
```

## Tests

Tests use [JUnit 6](https://junit.org/junit6/).

```shell
./gradlew test
```

## Dependencies

### Update Gradle

```shell
./gradlew wrapper --gradle-version=latest
```

## Version info

Version number is defined in [build.gradle.kts](./build.gradle.kts).
Version info (version number, git commit, branch, build time) is generated at compile time
by a custom Gradle `WriteVersionTask` that produces `VersionInfo.java` into the build directory.
