"""
Helper utilities for Othello game
Akseli Lukkarila
2019-2023
"""
from enum import IntEnum

try:
    # Python 3.11+
    from typing import Self
except ImportError:
    from typing_extensions import Self  # noqa: F401, UP035

from colorprint import Color, get_color


class Disk(IntEnum):
    """Represents one game piece or lack of one."""

    BLACK = -1
    EMPTY = 0
    WHITE = 1

    def board_char(self) -> str:
        """Returns a single character identifier string for the given disk."""
        if self.value == self.EMPTY:
            return "_"

        return get_color(self.name[0], self.color())

    def color(self) -> Color:
        """Return the associated color for this disk."""
        if self.value == self.EMPTY:
            return Color.white

        return Color.magenta if self.value == self.BLACK else Color.cyan

    def other_disk(self) -> Self:
        """Return the opposing disk color for this disk."""
        match self:
            case self.EMPTY:
                return self.EMPTY
            case self.BLACK:
                return self.WHITE
            case self.WHITE:
                return self.BLACK
            case _:
                raise NotImplementedError("Unknown disk type")

    def __str__(self) -> str:
        """Format disk name string with color."""
        return get_color(self.name, self.color())


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

    def __eq__(self, other) -> bool:
        if isinstance(other, Step):
            return self.x == other.x and self.y == other.y
        elif len(other) == 2:
            return self.x == other[0] and self.y == other[1]
        return NotImplemented

    def __ne__(self, other) -> bool:
        if isinstance(other, Step):
            return self.x != other.x or self.y != other.y
        elif len(other) == 2:
            return self.x != other[0] or self.y != other[1]
        return NotImplemented

    def __lt__(self, other) -> bool:
        if isinstance(other, Step):
            return self.x < other.x or (self.x <= other.x and self.y < other.y)
        elif len(other) == 2:
            return self.x < other[0] or (self.x <= other[0] and self.y < other[1])
        return NotImplemented

    def __le__(self, other) -> bool:
        if isinstance(other, Step):
            return self.x <= other.x and self.y < other.y
        elif len(other) == 2:
            return self.x <= other[0] and self.y < other[1]
        return NotImplemented

    def __gt__(self, other) -> bool:
        if isinstance(other, Step):
            return self.x > other.x or (self.x >= other.x and self.y > other.y)
        elif len(other) == 2:
            return self.x > other[0] or (self.x >= other[0] and self.y > other[1])
        return NotImplemented

    def __ge__(self, other) -> bool:
        if isinstance(other, Step):
            return self.x >= other.x and self.y >= other.y
        elif len(other) == 2:
            return self.x >= other[0] and self.y >= other[1]
        return NotImplemented

    def __str__(self) -> str:
        """Convert to string."""
        return f"[{self.x},{self.y}]"

    def __repr__(self) -> str:
        """Display Step with direction arrow."""
        # `match` does not work here without unpacking step :(
        if self == (-1, -1):
            return f"{self} \u2199"
        elif self == (-1, 0):
            return f"{self} \u2190"
        elif self == (-1, 1):
            return f"{self} \u2196"
        elif self == (0, -1):
            return f"{self} \u2193"
        elif self == (0, 1):
            return f"{self} \u2191"
        elif self == (1, -1):
            return f"{self} \u2198"
        elif self == (1, 0):
            return f"{self} \u2192"
        elif self == (1, 1):
            return f"{self} \u2197"

        return str(self)


class Square:
    """Represents one square location on the board."""

    def __init__(self, x: int, y: int):
        self.x: int = x
        self.y: int = y

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

    def __add__(self, other):
        # enable addition for a pair of squares,
        # or a square and an iterable with two values (square + tuple etc...)
        if isinstance(other, Square) or isinstance(other, Step):
            return Square(self.x + other.x, self.y + other.y)
        elif len(other) == 2:
            return Square(self.x + int(other[0]), self.y + int(other[1]))
        return NotImplemented

    def __iadd__(self, other) -> Self:
        # += operator
        return self + other

    def __radd__(self, other) -> Self:
        # other + self
        return other + self

    def __eq__(self, other) -> bool:
        return (self.x, self.y) == (other.x, other.y)

    def __ne__(self, other) -> bool:
        return (self.x, self.y) != (other.x, other.y)

    def __lt__(self, other) -> bool:
        return self.x < other.x or (self.x <= other.x and self.y < other.y)

    def __le__(self, other) -> bool:
        return self.x <= other.x and self.y < other.y

    def __gt__(self, other) -> bool:
        return self.x > other.x or (self.x >= other.x and self.y > other.y)

    def __ge__(self, other) -> bool:
        return self.x >= other.x and self.y >= other.y

    def __str__(self) -> str:
        return f"({self.x},{self.y})"


class Move:
    """Represents one possible disk placement for given disk color."""

    def __init__(self, square: Square, disk=Disk.EMPTY, value=0, directions=None):
        self.square: Square = square
        self.disk: Disk = disk
        self.value: int = value
        self.directions: list[Step] = directions

    def __eq__(self, other) -> bool:
        return (self.square, self.value) == (other.square, other.value)

    def __ne__(self, other) -> bool:
        return (self.square, self.value) != (other.square, other.value)

    def __lt__(self, other) -> bool:
        return self.value > other.value or (
            self.value == other.value and self.square < other.square
        )

    def __le__(self, other) -> bool:
        return self.value >= other.value and self.square <= other.square

    def __gt__(self, other) -> bool:
        return self.value < other.value or (
            self.value == other.value and self.square > other.square
        )

    def __ge__(self, other) -> bool:
        return self.value <= other.value and self.square >= other.square

    def __str__(self) -> str:
        return f"Square: {self.square} -> value: {self.value}"


def clamp(value, minimum, maximum):
    """Clamp value to given range."""
    assert minimum <= maximum, "Minimum value should be less than or equal to maximum!"
    return max(minimum, min(value, maximum))
