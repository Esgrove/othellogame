# Othello Python

[![Poetry](https://img.shields.io/endpoint?url=https://python-poetry.org/badge/v0.json)](https://python-poetry.org/)
[![black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
[![Ruff](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/astral-sh/ruff/main/assets/badge/v2.json)](https://github.com/astral-sh/ruff)

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
# through poetry
poetry run python othello/othello.py
# or with helper script
./othello.sh
# executing directly also works in a unix shell
./othello/othello.py
```

**Note:** Windows uses `python` by default for Python 3,
whereas Mac will only have `python3` in path unless you are using pyenv for example.

There is a Batch script [othello.bat](./othello.bat) for easily running it on Windows.
