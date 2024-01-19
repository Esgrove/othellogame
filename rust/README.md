# Othello Rust

Othello [Rust](https://www.rust-lang.org/) implementation.

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
