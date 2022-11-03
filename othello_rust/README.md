# Othello Rust

## Build

Rust and Cargo make things really easy:

```console
cargo build
cargo build --release
cargo run
```

## Format code

Using [rustfmt](https://github.com/rust-lang/rustfmt)

```console
rustup component add rustfmt
cargo fmt
```

## Lint

Using [Clippy](https://github.com/rust-lang/rust-clippy)

```console
rustup component add clippy
cargo clippy
cargo clippy --fix
```

## Update dependencies

```console
cargo update
```
