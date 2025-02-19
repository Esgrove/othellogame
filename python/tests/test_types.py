import pytest

from othello.types import Disk, Square, Move


@pytest.fixture
def white():
    return Disk.WHITE


@pytest.fixture
def black():
    return Disk.BLACK


@pytest.fixture
def empty():
    return Disk.EMPTY


@pytest.fixture
def square():
    return Square(1, 1)


@pytest.fixture
def move():
    return Move(Square(1, 1), disk=Disk.WHITE, value=1)


def test_disk_other_disk_for_white(white):
    assert white.opponent() == Disk.BLACK


def test_disk_other_disk_for_black(black):
    assert black.opponent() == Disk.WHITE


def test_disk_other_disk_for_empty(empty):
    assert empty.opponent() == Disk.EMPTY


def test_disk_get_board_string(empty):
    assert empty.board_char() == "_"


def test_square_string(square):
    assert str(square) == "(1,1)"


def test_move_string(move):
    assert str(move) == "Square: (1,1) -> value: 1"


def test_move_equal_operator(move):
    assert move == Move(Square(1, 1), value=1)
    assert not move == Move(Square(1, 1), value=0)
    assert not move == Move(Square(1, 1), value=2)
    assert not move == Move(Square(0, 1), value=1)
    assert not move == Move(Square(0, 0), value=1)
    assert not move == Move(Square(0, 0), value=0)


def test_square_equal_operator(square):
    assert square == Square(1, 1)
    assert not square == Square(0, 0)
    assert not square == Square(1, 0)
    assert not square == Square(0, 1)


def test_square_not_equal_operator(square):
    assert square != Square(0, 0)
    assert square != Square(0, 1)
    assert square != Square(1, 0)
    assert not square != Square(1, 1)


def test_square_addition(square):
    square_minus_one = Square(-1, -1)
    square_zero = Square(0, 0)
    square_two = Square(2, 2)
    assert square == square + square_zero
    assert square + square_minus_one == square_zero
    assert square + square == square_two
    square_two += square_minus_one
    assert square_two == square
