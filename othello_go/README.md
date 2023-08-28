# Othello GO

Othello [Go](https://go.dev/) implementation.

## Run

```shell
go run .
```

## Build

```shell
go build -race -v
```

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
