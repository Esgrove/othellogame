# Othello Swift

## Usage

```console
OVERVIEW: A simple Othello CLI game implementation in Swift

USAGE: othello_swift [options] [size]

ARGUMENTS:
  <size>                  Optional board size (4..10)

OPTIONS:
  -a, --autoplay          Enable autoplay mode with computer control
  -c, --check             Autoplay and only print result
  -u, --default           Play with default settings
  -l, --log               Show game log at the end
  -n, --no-helpers        Hide disk placement hints
  -t, --test              Enable test mode with deterministic computer moves
  -v, --version           Print version and exit
  -h, --help              Show help information.
```

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

## Tests

Tests use [Swift Testing](https://developer.apple.com/documentation/testing).

```shell
swift test
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

## Update Swift version

Update the `swift-tools-version` comment in [Package.swift](./Package.swift):

```swift
// swift-tools-version:6.3
```

## Version info

The app version number is defined in a single source of truth: [VERSION](./VERSION).
Version info (version number, git commit, branch, build time) is injected at compile time
using a [SwiftPM build tool plugin](./Plugins/VersionGeneratorPlugin/plugin.swift)
that invokes a [shell script](./Plugins/VersionGeneratorPlugin/generate_version.sh)
to generate `versionInfo.swift` automatically during `swift build`.
