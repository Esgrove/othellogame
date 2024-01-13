# Othello Swift

## Build

```shell
swift build
swift build --configuration release

swift run
swift run --configuration release
```

Or using script:

```shell
./build.sh
```

## Format

```shell
brew install swiftformat
swiftformat .
```

Configuration in [.swiftformat](./.swiftformat)

## Update dependencies

```shell
swift package update
swift package resolve
```
