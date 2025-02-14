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
