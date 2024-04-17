# Othello GO

Othello [Go](https://go.dev/) implementation.

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
