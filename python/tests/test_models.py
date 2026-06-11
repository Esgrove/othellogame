import pytest

from othello.models import Direction, Disk, Move, Square, Step


@pytest.fixture
def square():
    return Square(1, 1)


@pytest.fixture
def move():
    return Move(Square(1, 1), disk=Disk.WHITE, value=1)


def test_step_addition():
    result = Step(0, 0) + Step(1, 1)
    assert result == Step(1, 1)

    result = Step(-1, 0) + Step(1, 0)
    assert result == Step(0, 0)

    result += Step(-1, -1)
    assert result == Step(-1, -1)

    result += Step(1, 1)
    assert result == Step(0, 0)


def test_square_addition():
    result = Square(4, 4) + Square(1, 1)
    assert result == Square(5, 5)

    result = Square(4, 4) + Square(0, 0)
    assert result == Square(4, 4)

    result = Square(4, 4) + Step(-1, 1)
    assert result == Square(3, 5)

    result += Square(0, 0)
    assert result == Square(3, 5)

    another = Square(-3, -2) + Square(2, 3)
    assert another == Square(-1, 1)

    result += Step(-1, -1)
    assert result == Square(2, 4)

    result += Step(-1, -1)
    assert result == Square(1, 3)

    result += Step(-1, -1)
    assert result == Square(0, 2)

    result += Step(-1, -1)
    assert result == Square(-1, 1)

    result += Step(1, -1)
    assert result == Square(0, 0)

    result += Step(-1, -1)
    assert result == Square(-1, -1)


def test_step_directions():
    origin = Square(1, 1)
    step_directions = [
        Step(-1, -1),
        Step(-1, 0),
        Step(-1, 1),
        Step(0, -1),
        Step(0, 1),
        Step(1, -1),
        Step(1, 0),
        Step(1, 1),
    ]
    expected_results = [
        Square(0, 0),
        Square(0, 1),
        Square(0, 2),
        Square(1, 0),
        Square(1, 2),
        Square(2, 0),
        Square(2, 1),
        Square(2, 2),
    ]
    for step, expected in zip(step_directions, expected_results):
        result = origin + step
        assert result == expected, f"Step direction: {step}"


def test_disk_board_char():
    assert Disk.BLACK.board_char() == "B"
    assert Disk.EMPTY.board_char() == "_"
    assert Disk.WHITE.board_char() == "W"


def test_disk_opponent():
    assert Disk.BLACK.opponent() == Disk.WHITE
    assert Disk.EMPTY.opponent() == Disk.EMPTY
    assert Disk.WHITE.opponent() == Disk.BLACK


def test_move_log_entry():
    b = Move(
        square=Square(3, 2),
        disk=Disk.BLACK,
        value=10,
        directions=[Direction(Step(1, 0), 10)],
    )
    assert b.log_entry() == "B:(3,2),10"

    w = Move(
        square=Square(0, 0),
        disk=Disk.WHITE,
        value=1,
        directions=[Direction(Step(1, 0), 1)],
    )
    assert w.log_entry() == "W:(0,0),1"


def test_square_string(square):
    assert str(square) == "(1,1)"


def test_move_string(move):
    assert str(move) == "Square: (1,1) -> value: 1"


def test_move_equal_operator(move):
    assert move == Move(Square(1, 1), disk=Disk.WHITE, value=1)
    # Equality includes the disk
    assert not move == Move(Square(1, 1), disk=Disk.BLACK, value=1)
    assert not move == Move(Square(1, 1), disk=Disk.WHITE, value=0)
    assert not move == Move(Square(1, 1), disk=Disk.WHITE, value=2)
    assert not move == Move(Square(0, 1), disk=Disk.WHITE, value=1)
    assert not move == Move(Square(0, 0), disk=Disk.WHITE, value=1)
    assert not move == Move(Square(0, 0), disk=Disk.WHITE, value=0)


def test_move_hash(move):
    assert hash(move) == hash(Move(Square(1, 1), disk=Disk.WHITE, value=1))
    assert hash(move) != hash(Move(Square(1, 1), disk=Disk.BLACK, value=1))
    assert hash(move) != hash(Move(Square(1, 1), disk=Disk.WHITE, value=2))


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


def test_square_comparison(square):
    assert square < Square(2, 1)
    assert square < Square(1, 2)
    assert square <= Square(1, 1)
    assert square >= Square(1, 1)
    assert square > Square(0, 1)
    assert square > Square(1, 0)


def test_square_board_index(square):
    assert square.board_index(8) == 9
    assert Square(0, 0).board_index(8) == 0
    assert Square(7, 7).board_index(8) == 63
    assert Square(3, 2).board_index(4) == 11
