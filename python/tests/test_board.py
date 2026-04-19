import pytest

from othello.board import Board
from othello.models import Disk, Square


@pytest.fixture
def board_default():
    return Board(8)


def test_board_initialization():
    board_size = 8
    board = Board(board_size)
    assert board._size == board_size

    # Check initial disk positions
    assert board._get_square(Square(3, 3)) == Disk.WHITE
    assert board._get_square(Square(4, 4)) == Disk.WHITE
    assert board._get_square(Square(3, 4)) == Disk.BLACK
    assert board._get_square(Square(4, 3)) == Disk.BLACK

    # Rest of the board should be empty
    assert board._get_square(Square(0, 0)) == Disk.EMPTY
    assert board._get_square(Square(7, 7)) == Disk.EMPTY


def test_can_play():
    board = Board(8)
    assert board.can_play() is True


def test_scoring():
    board = Board(8)
    black_score, white_score = board._player_scores()
    assert black_score == 2
    assert white_score == 2
    assert board._score() == 0


def test_game_result():
    board = Board(8)
    result = board.result()
    assert result == Disk.EMPTY

    moves = board.possible_moves(Disk.BLACK)
    board.place_disk(moves[0])

    result = board.result()
    assert result == Disk.BLACK


def test_log_entry():
    board = Board(8)
    log = board.log_entry()
    assert log == "___________________________WB______BW___________________________"

    board4 = Board(4)
    log4 = board4.log_entry()
    assert log4 == "_____WB__BW_____"

    moves = board4.possible_moves(Disk.BLACK)
    board4.place_disk(moves[0])
    log4_after = board4.log_entry()
    assert log4_after == "____BBB__BW_____"
