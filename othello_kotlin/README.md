# Othello Kotlin

Requires Gradle.

Uses [Clikt](https://github.com/ajalt/clikt) for the CLI.

## Build and run

```shell
./gradlew build

./gradlew run
```

With shell script:

```shell
./build.sh
```

The build script will produce a standalone jar using [Shadow](https://github.com/johnrengelman/shadow)
that can then be run with:

```shell
java -jar othello_kotlin.jar
```

## Code formatting

Using [ktlint](https://github.com/pinterest/ktlint):

```shell
brew install ktlint

ktlint --format
```

## Todo

- Use [Arrow](https://arrow-kt.io) for Rust style error handling
