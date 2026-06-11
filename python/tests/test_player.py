from othello.models import Disk
from othello.player import Player, PlayerType
from othello.settings import PlayerSettings


def test_new_player():
    player = Player(Disk.BLACK)
    assert player._disk == Disk.BLACK
    assert player.human() is True
    assert player.computer() is False
    assert player.can_play is True
    assert player._rounds_played == 0
    assert player._settings == PlayerSettings.default()


def test_reset_player():
    player = Player(Disk.WHITE, PlayerSettings.default())
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
    assert player._player_type == PlayerType.COMPUTER

    player.set_human()
    assert player.human() is True
    assert player._player_type == PlayerType.HUMAN


def test_player_type_string():
    player = Player(Disk.BLACK)
    assert player.type_string() == "Human   "

    player.set_computer()
    assert player.type_string() == "Computer"
