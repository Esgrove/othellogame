import pytest

from util import Disk, Move, Square, clamp


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
    return Move(Square(1, 1), value=1)


def test_disk_other_disk_for_white(white):
    assert white.other_disk() == Disk.BLACK


def test_disk_other_disk_for_black(black):
    assert black.other_disk() == Disk.WHITE


def test_disk_other_disk_for_empty(empty):
    assert empty.other_disk() == Disk.EMPTY


def test_disk_get_board_string(empty):
    assert empty.get_board_char() == "_"


def test_square_string(square):
    assert str(square) == "(1,1)"


def test_move_string(move):
    assert str(move) == "point: (1,1) -> value: 1"


def test_move_equal_operator(move):
    assert move == Move(Square(1, 1), 1)
    assert not move == Move(Square(1, 1), 0)
    assert not move == Move(Square(1, 1), 2)
    assert not move == Move(Square(0, 1), 1)
    assert not move == Move(Square(0, 0), 1)
    assert not move == Move(Square(0, 0), 0)


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


def test_clamp():
    assert clamp(0, 0, 0) == 0
    assert clamp(0, 0, 1) == 0
    assert clamp(0, 1, 1) == 1
    assert clamp(0, 1, 2) == 1
    assert clamp(0, -1, 0) == 0
    assert clamp(0, -1, 1) == 0
    assert clamp(0, -1, -1) == -1
    assert clamp(0, -2, -1) == -1

    assert clamp(1, 0, 0) == 0
    assert clamp(1, 0, 1) == 1
    assert clamp(1, 0, 2) == 1
    assert clamp(1, -1, 1) == 1
    assert clamp(1, -1, 2) == 1
    assert clamp(1, -1, 0) == 0

    assert clamp(0.0, 0.0, 0.0) == 0.0
    assert clamp(0.0, 0.0, 1.0) == 0.0
    assert clamp(0.0, 1.0, 1.0) == 1.0
    assert clamp(0.0, 1.0, 2.0) == 1.0
    assert clamp(0.0, -1.0, 0.0) == 0.0
    assert clamp(0.0, -1.0, 1.0) == 0.0

    assert clamp(0.5, 0.0, 1.0) == 0.5
    assert clamp(0.5, 0.5, 1.0) == 0.5
    assert clamp(0.5, 0.0, 0.0) == 0.0
    assert clamp(0.5, -1.0, 0.0) == 0.0

    # bigger min value should raise AssertionError
    with pytest.raises(AssertionError):
        assert clamp(0, 1, 0)
        assert clamp(0, 0, -1)
        assert clamp(0, 1, -1)
        assert clamp(0, -1, -2)
