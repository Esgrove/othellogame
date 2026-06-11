"""
Models
Basic types and methods
Akseli Lukkarila
2019-2026
"""

from enum import IntEnum
from functools import total_ordering
from typing import Self

try:
    from othello.colorprint import Color, get_color
except ModuleNotFoundError:
    from colorprint import Color, get_color


class Disk(IntEnum):
    """Represents one game piece or lack of one."""

    BLACK = -1
    EMPTY = 0
    WHITE = 1

    def board_char(self) -> str:
        """Returns a single character identifier string for the given disk."""
        match self:
            case self.BLACK:
                return "B"
            case self.EMPTY:
                return "_"
            case self.WHITE:
                return "W"
            case _:
                raise NotImplementedError("Unknown disk type")

    def board_char_with_color(self) -> str:
        """Returns a single character identifier string for the given disk."""
        return get_color(self.board_char(), self.disk_color())

    def disk_color(self) -> Color:
        """Return the associated colour for this disk."""
        match self:
            case self.BLACK:
                return Color.magenta
            case self.EMPTY:
                return Color.white
            case self.WHITE:
                return Color.cyan
            case _:
                raise NotImplementedError("Unknown disk type")

    def disk_string(self) -> str:
        """Returns the disk formatted as a coloured string."""
        return get_color(self.name, self.disk_color())

    def opponent(self) -> Self:
        """Return the opposing disk colour for this disk."""
        match self:
            case self.BLACK:
                return self.WHITE
            case self.EMPTY:
                return self.EMPTY
            case self.WHITE:
                return self.BLACK
            case _:
                raise NotImplementedError("Unknown disk type")

    def __str__(self) -> str:
        return self.disk_string()


@total_ordering
class Step:
    """Represents one step direction on the board."""

    def __init__(self, x: int, y: int):
        self.x: int = x
        self.y: int = y

    def __getitem__(self, key) -> int:
        # enable iteration so values can be unpacked: x,y = Step
        if key == 0:
            return self.x
        elif key == 1:
            return self.y
        raise IndexError

    def __hash__(self) -> int:
        # needed to enable using this class in sets or as a dictionary key
        return hash((self.x, self.y))

    def __add__(self, other) -> Self:
        if isinstance(other, Step):
            return Step(self.x + other.x, self.y + other.y)
        elif len(other) == 2:
            return Step(self.x + int(other[0]), self.y + int(other[1]))
        return NotImplemented

    def __iadd__(self, other) -> Self:
        # += operator
        return self + other

    def __eq__(self, other) -> bool:
        if isinstance(other, Step):
            # another way: `(self.x, self.y) == (other.x, other.y)`
            return self.x == other.x and self.y == other.y
        elif len(other) == 2:
            return self.x == other[0] and self.y == other[1]
        return NotImplemented

    def __lt__(self, other) -> bool:
        if isinstance(other, Step):
            return self.x < other.x or (self.x == other.x and self.y < other.y)
        elif len(other) == 2:
            return self.x < other[0] or (self.x == other[0] and self.y < other[1])
        return NotImplemented

    def __str__(self) -> str:
        """Convert to string."""
        return f"[{self.x},{self.y}]"

    def __repr__(self) -> str:
        """Display Step with direction arrow."""
        # `match` does not work here without the unpacking step :(
        if self == (-1, -1):
            return f"{self} ↙"
        elif self == (-1, 0):
            return f"{self} ←"
        elif self == (-1, 1):
            return f"{self} ↖"
        elif self == (0, -1):
            return f"{self} ↓"
        elif self == (0, 1):
            return f"{self} ↑"
        elif self == (1, -1):
            return f"{self} ↘"
        elif self == (1, 0):
            return f"{self} →"
        elif self == (1, 1):
            return f"{self} ↗"

        return str(self)


@total_ordering
class Square:
    """Represents one square location on the board."""

    def __init__(self, x: int, y: int):
        self.x: int = x
        self.y: int = y

    def board_index(self, board_size: int) -> int:
        """Get the index of this square on the board."""
        return self.y * board_size + self.x

    def __getitem__(self, key) -> int:
        # enable iteration so coordinates can be unpacked: x,y = Square
        if key == 0:
            return self.x
        elif key == 1:
            return self.y
        raise IndexError

    def __hash__(self) -> int:
        # needed to enable using this class in sets or as a dictionary key
        return hash((self.x, self.y))

    def __add__(self, other) -> Self:
        # enable addition for a pair of squares,
        # or a square and an iterable with two values (square + tuple etc...)
        if isinstance(other, (Square, Step)):
            return Square(self.x + other.x, self.y + other.y)
        elif len(other) == 2:
            return Square(self.x + int(other[0]), self.y + int(other[1]))
        return NotImplemented

    def __iadd__(self, other) -> Self:
        # += operator
        return self + other

    def __radd__(self, other) -> Self:
        # other + self
        return self + other

    def __eq__(self, other) -> bool:
        if isinstance(other, Square):
            return self.x == other.x and self.y == other.y
        elif len(other) == 2:
            return self.x == other[0] and self.y == other[1]
        return NotImplemented

    def __lt__(self, other) -> bool:
        if isinstance(other, Square):
            return self.x < other.x or (self.x == other.x and self.y < other.y)
        elif len(other) == 2:
            return self.x < other[0] or (self.x == other[0] and self.y < other[1])
        return NotImplemented

    def __str__(self) -> str:
        return f"({self.x},{self.y})"


@total_ordering
class Direction:
    """Represents a continuous line of squares in one direction.

    The step field determines the direction on the board,
    and count describes how many consecutive squares in that direction there are.
    """

    def __init__(self, step: Step, count: int):
        self.step: Step = step
        self.count: int = count

    def __getitem__(self, key):
        # Enable iteration so values can be unpacked: step, count = Direction
        if key == 0:
            return self.step
        elif key == 1:
            return self.count
        raise IndexError

    def __hash__(self) -> int:
        return hash((self.step, self.count))

    def __eq__(self, other) -> bool:
        if isinstance(other, Direction):
            return self.step == other.step and self.count == other.count
        return NotImplemented

    def __lt__(self, other) -> bool:
        if isinstance(other, Direction):
            return self.step < other.step or (self.step == other.step and self.count < other.count)
        return NotImplemented

    def __str__(self) -> str:
        return f"{self.step}:{self.count}"


@total_ordering
class Move:
    """Represents one possible disk placement for the given disk colour."""

    def __init__(self, square: Square, disk=Disk.EMPTY, value=0, directions=None):
        self.square: Square = square
        self.disk: Disk = disk
        self.value: int = value
        self.directions: list[Direction] = directions if directions is not None else []

    def log_entry(self) -> str:
        """Format move for log entry."""
        return f"{self.disk.board_char()}:{self.square},{self.value}"

    def affected_squares(self) -> list[Square]:
        """Get all the squares playing this move will change."""
        paths = []
        for step, count in self.directions:
            pos = self.square + step
            for _ in range(count):
                paths.append(pos)
                pos += step

        return sorted(paths)

    def __hash__(self) -> int:
        # Note: hash uses the same fields as equality: square, value, disk
        return hash((self.square, self.value, self.disk))

    def __eq__(self, other) -> bool:
        if isinstance(other, Move):
            return (
                self.square == other.square
                and self.value == other.value
                and self.disk == other.disk
            )
        return NotImplemented

    def __lt__(self, other) -> bool:
        # Moves are ordered in descending order by value, and ascending order by square
        return self.value > other.value or (
            self.value == other.value and self.square < other.square
        )

    def __str__(self) -> str:
        return f"Square: {self.square} -> value: {self.value}"
