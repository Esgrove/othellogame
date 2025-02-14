# Othello Rust

Othello [Rust](https://www.rust-lang.org/) implementation.

## Usage

```console
A simple Othello CLI game implementation in Rust

Usage: othello_rust [OPTIONS] [SIZE]

Arguments:
  [SIZE]  Optional board size (4..10)

Options:
  -a, --autoplay    Enable autoplay mode with computer control
  -c, --check       Autoplay and only print result
  -d, --default     Play with default settings
  -l, --log         Show game log at the end
  -n, --no-helpers  Hide disk placement hints
  -t, --test        Enable test mode with deterministic computer moves
  -v, --version     Print version and exit
  -h, --help        Print help (see more with '--help')
```

## Build

Using script:

```shell
./build.sh
```

Manually:

```shell
cargo build
cargo build --release

cargo run -- --help
cargo run --release
```

## Tests

```shell
cargo test
```

## Format code

Using [rustfmt](https://github.com/rust-lang/rustfmt)

```shell
rustup component add rustfmt
cargo fmt
```

## Lint

Using [Clippy](https://github.com/rust-lang/rust-clippy)

```shell
rustup component add clippy
cargo clippy
cargo clippy --fix
```

## Update dependencies

```shell
cargo update
```
