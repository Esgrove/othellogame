# Othello TypeScript

TypeScript implementation using Bun.

## Usage

```console
Usage: Othello TypeScript [options] [SIZE]

A simple Othello CLI game implementation.

Arguments:
  SIZE              Optional board size (4..10)

Options:
  -a, --autoplay    Enable autoplay mode (default: false)
  -c, --check       Autoplay and only print result (default: false)
  -d, --default     Play with default settings (default: false)
  -l, --log         Show log after a game (default: false)
  -n, --no-helpers  Hide disk placement hints
  -t, --test        Enable test mode (default: false)
  -v, --version     Print version and exit
  -h, --help        Print usage and exit
```

## Build and run

Using scripts:

```shell
bun run build
bun othello
bun othello --help
```

Directly with Bun:

```shell
bun src/othello.ts --help
```

## Tests

Tests use the built-in [Bun test runner](https://bun.sh/docs/cli/test).

```shell
bun test
```

## Version info

Version number is defined in [package.json](./package.json).
The version is read at runtime from `package.json`.
