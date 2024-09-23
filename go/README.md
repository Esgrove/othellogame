# Othello GO

Othello [Go](https://go.dev/) implementation.

## Usage

```console
A simple Othello CLI game implementation.

Usage:
  othello_go [SIZE] [flags]

Arguments:
  [SIZE]             Optional board size (4..10)

Flags:
  -a, --autoplay     Enable autoplay mode
  -d, --default      Play with default settings
  -h, --help         help for othello_go
  -l, --log          Show log after a game
  -n, --no-helpers   Hide disk placement hints
  -t, --test         Enable test mode
  -v, --version      Print version and exit
```

## Run

```shell
go run .
```

## Build

Using script:

```shell
./build.sh
```

Manually:

```shell
go build -v
```

Note that version information only works when building with script since that sets the required variables.

## Install

```shell
go install
```

Check binary install target:

```shell
go list -f '{{.Target}}'
```

## Format code

Using [gofmt](https://pkg.go.dev/cmd/gofmt)

```shell
gofmt -s -w .
```

## Lint code

Using [golangci-lint](https://golangci-lint.run/)

```shell
brew install golangci-lint

golangci-lint run
```

## Update dependencies

```shell
# check for available updates
go list -u -m all
# update a specific package
go get -u example.com/pkg
# update all dependencies
go get -u ./...
# cleanup
go mod tidy
go mod verify
```
