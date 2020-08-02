import pytest

from board import Board
from utils import Disk


@pytest.fixture
def board_default():
    return Board(8)

