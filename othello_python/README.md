# Othello Python

[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)

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

## Run

```shell
python3 othello/othello.py
# or with helper script
./othello.sh
```

In a unix shell (bash etc.), executing directly also works:

```shell
./othello/othello.py
```

**Note:** Windows uses `python` by default for Python 3,
whereas Mac will only have `python3` in path unless you are using pyenv for example.

There is a Batch script [othello.bat](./othello.bat) for easily running it on Windows.
