import pytest

from othello.board import Board
from othello.player import Player
from othello.models import Disk


@pytest.fixture
def white_human():
    return Player(Disk.WHITE)


@pytest.fixture
def black_human():
    return Player(Disk.BLACK)


@pytest.fixture
def white_computer():
    return Player(Disk.WHITE)


@pytest.fixture
def black_computer():
    return Player(Disk.BLACK)


@pytest.fixture
def board_start_8():
    return Board(8)
