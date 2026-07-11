# Othello TypeScript

TypeScript implementation using [Bun](https://bun.sh).

## Usage

```console
Usage: othello_typescript [options] [SIZE]

A simple Othello CLI game implementation in TypeScript

Arguments:
  SIZE              Optional board size (4..10)

Options:
  -a, --autoplay    Enable autoplay mode with computer control
  -c, --check       Autoplay and only print result
  -d, --default     Play with default settings
  -l, --log         Show game log at the end
  -n, --no-helpers  Hide disk placement hints
  -t, --test        Enable test mode with deterministic computer moves
  -v, --version     Print version and exit
  -h, --help        Print help and exit
```

## Build and run

Build with the build script,
which installs dependencies and updates the version information:

```shell
./build.sh
```

Run with the launcher script:

```shell
./othello_typescript.sh
```

Or directly with Bun:

```shell
bun src/main.ts --help
bun othello
```

## Tests

Tests use the built-in [Bun test runner](https://bun.sh/docs/cli/test).

```shell
bun test
```

## Code formatting and linting

Uses [oxfmt](https://oxc.rs/docs/guide/usage/formatter.html) and
[oxlint](https://oxc.rs/docs/guide/usage/linter.html).
Linting runs the TypeScript compiler typecheck first.

```shell
bun format
bun lint
bun typecheck
```

## Version info

Version number is defined in [package.json](./package.json).
Version info (version number, git commit, branch, build time) is generated
by the [build.sh](./build.sh) script, which writes `src/version.ts`.
