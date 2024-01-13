import pytest

from othello.board import Board


@pytest.fixture
def board_default():
    return Board(8)
