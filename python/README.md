# Othello Python

[![Poetry](https://img.shields.io/endpoint?url=https://python-poetry.org/badge/v0.json)](https://python-poetry.org/)
[![Ruff](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/astral-sh/ruff/main/assets/badge/v2.json)](https://github.com/astral-sh/ruff)

## Usage

```console
Usage: othello.py [OPTIONS] [SIZE]

  A simple Othello CLI game implementation in Python

  [SIZE]  Optional board size (4..10)

Options:
  -a, --autoplay    Enable autoplay mode with computer control
  -c, --check       Autoplay and only print result
  -d, --default     Play with default settings
  -l, --log         Show game log at the end
  -n, --no-helpers  Hide disk placement hints
  -t, --test        Enable test mode with deterministic computer moves
  -v, --version     Print version and exit
  -h, --help        Show this message and exit.
```

## Dependencies

Requires Python 3.11+

Dependencies are managed by [Poetry](https://python-poetry.org/docs/).

Install dependencies:

```shell
poetry install
```

Update dependencies:

```shell
poetry update
```

Activate virtual env:

```shell
poetry shell
```

## Run

```shell
# with virtual env active
python3 othello/othello.py
# with poetry
poetry run python othello/othello.py
# with poetry script
poetry run othello
# or with helper script
./othello_python.sh
# executing directly also works in a unix shell
./othello/othello.py
```

**Note:** Windows uses `python` by default for Python 3,
whereas Mac / Unix will only have `python3` in path unless you are using pyenv for example.

There is a Batch script [othello.bat](./othello.bat) for easily running it on Windows.

## Tests

```shell
poetry run pytest -v
# with coverage
poetry run pytest --cov=othello tests
```

Shortcut:

```shell
./test.sh
```

## TODO

- Swap from Poetry to uv
- Add more test cases
