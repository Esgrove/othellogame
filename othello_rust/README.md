# Othello Rust

Othello [Rust](https://www.rust-lang.org/) implementation.

## Build

Rust and Cargo make things really easy:

```shell
cargo build
cargo build --release
cargo run
cargo run --release
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
