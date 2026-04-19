import pytest

from othello.board import Board
from othello.player import Player
from othello.models import Disk, PlayerType


@pytest.fixture
def white_human():
    return Player(Disk.WHITE)


@pytest.fixture
def black_human():
    return Player(Disk.BLACK)


@pytest.fixture
def white_computer():
    return Player(Disk.WHITE, player_type=PlayerType.COMPUTER)


@pytest.fixture
def black_computer():
    return Player(Disk.BLACK, player_type=PlayerType.COMPUTER)


@pytest.fixture
def board_start_8():
    return Board(8)


def test_new_player():
    player = Player(Disk.BLACK)
    assert player._disk == Disk.BLACK
    assert player.human() is True
    assert player.computer() is False
    assert player.can_play is True
    assert player._rounds_played == 0


def test_reset_player():
    player = Player(Disk.WHITE)
    player.can_play = False
    player._rounds_played = 10
    player.reset()
    assert player.can_play is True
    assert player._rounds_played == 0


def test_set_human():
    player = Player(Disk.BLACK)
    assert player.human() is True
    assert player.computer() is False

    player.set_computer()
    assert player.computer() is True
    assert player.human() is False

    player.set_human()
    assert player.human() is True
    assert player.computer() is False


def test_player_type_string():
    player = Player(Disk.BLACK)
    assert player.human() is True
    assert player._type_string() == "Human   "

    player.set_computer()
    assert player.computer() is True
    assert player._type_string() == "Computer"
