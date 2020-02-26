import pytest

from board import Board
from player import Player
from util import Disk


@pytest.fixture
def white_human():
    return Player(Disk.WHITE)


@pytest.fixture
def black_human():
    return Player(Disk.BLACK)


@pytest.fixture
def white_computer():
    return Player(Disk.WHITE, human=False)


@pytest.fixture
def black_computer():
    return Player(Disk.BLACK, human=False)


@pytest.fixture
def board_start_8():
    return Board(8)
