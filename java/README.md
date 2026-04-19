# Othello Java

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
