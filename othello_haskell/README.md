# Othello Haskell

Uses:

- [Cabal](https://www.haskell.org/cabal) for building
- [Ormolu](https://github.com/tweag/ormolu) for code formatting

## Build

```bash
cabal build
```

```bash
cabal run
```

## Format code

```bash
cabal install ormolu
```

```bash
ormolu --mode inplace **/*.hs
```
