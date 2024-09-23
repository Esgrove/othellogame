# Othello Swift

## Usage

```console
OVERVIEW: A simple Othello CLI game implementation.

USAGE: othello_swift [options] [size]

ARGUMENTS:
  <size>                  Optional board size (4..10)

OPTIONS:
  -a, --autoplay          Enable autoplay mode
  -u, --default           Play with default settings
  -l, --log               Show log after a game
  -n, --no-helpers        Hide disk placement hints
  -t, --test              Enable test mode
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
