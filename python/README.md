# Othello Python

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

Dependencies are managed with uv.

Install dependencies:

```shell
uv sync --all-groups
```

Update dependencies:

```shell
uv sync --all-groups --upgrade
```

## Run

```shell
# with virtual env active
python3 othello/othello.py
# with script
uv run othello
# with uv
uv run othello/othello.py
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
uv run pytest -v
# with coverage
uv run pytest --cov=othello tests
```

Script:

```shell
./test.sh
```

## Formatting and linting

````bash
uv run ruff format
uv run ruff check --fix
````

## TODO

- Add more test cases
