# Othello GO

Othello [Go](https://go.dev/) implementation.

## Run

```shell
go run .
```

## Build

Manually:

```shell
go build -race -v
```

Or using script:

```shell
./build.sh
```

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

## Update version number

```shell
go generate
```

Or with script:

```shell
./update_version.sh
```

Override version manually:

```shell
VERSION=1.2.3 ./update_version.sh
```
