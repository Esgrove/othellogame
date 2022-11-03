"""
Helper utilities for Othello game
Akseli Lukkarila
2019
"""
from enum import IntEnum

from colorprint import Color, get_color


class Disk(IntEnum):
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

    def other_disk(self):
        """Return the opposing disk color for this disk."""
        if self.value == self.EMPTY:
            return self.EMPTY

        return self.WHITE if self.value == self.BLACK else self.BLACK

    def __str__(self):
        """Format disk name string with color."""
        return get_color(self.name, self.color())


class Square:
    """Represents one square location on the board."""

    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __getitem__(self, key):
        # enable iteration so coordinates can be unpacked: x,y = Square
        if key == 0:
            return self.x
        elif key == 1:
            return self.y
        raise IndexError

    def __hash__(self):
        # needed to enable using this class in sets or as a dictionary key
        return hash((self.x, self.y))

    def __add__(self, other):
        # enable addition for a pair of squares or a square and an iterable with two values (square + tuple etc...)
        if isinstance(other, Square):
            return Square(self.x + other.x, self.y + other.y)
        elif len(other) == 2:
            return Square(self.x + other[0], self.y + other[1])
        return NotImplemented

    def __iadd__(self, other):
        # += operator
        return self + other

    def __radd__(self, other):
        # other + self
        return self + other

    def __eq__(self, other):
        return (self.x, self.y) == (other.x, other.y)

    def __ne__(self, other):
        return (self.x, self.y) != (other.x, other.y)

    def __lt__(self, other):
        return self.x < other.x or (self.x <= other.x and self.y < other.y)

    def __le__(self, other):
        return self.x <= other.x and self.y < other.y

    def __gt__(self, other):
        return self.x > other.x or (self.x >= other.x and self.y > other.y)

    def __ge__(self, other):
        return self.x >= other.x and self.y >= other.y

    def __str__(self):
        return f"({self.x},{self.y})"


class Move:
    """Represents one possible disk placement for given disk color."""

    def __init__(self, square: Square, disk=Disk.EMPTY, value=0, directions=None):
        self.square = square
        self.disk = disk
        self.value = value
        self.directions = directions

    def __eq__(self, other):
        return (self.square, self.value) == (other.square, other.value)

    def __ne__(self, other):
        return (self.square, self.value) != (other.square, other.value)

    def __lt__(self, other):
        return self.value > other.value or (self.value == other.value and self.square < other.square)

    def __le__(self, other):
        return self.value >= other.value and self.square <= other.square

    def __gt__(self, other):
        return self.value < other.value or (self.value == other.value and self.square > other.square)

    def __ge__(self, other):
        return self.value <= other.value and self.square >= other.square

    def __str__(self):
        return f"Square: {self.square} -> value: {self.value}"


def clamp(number, minimum, maximum):
    """Clamp value to given range."""
    assert minimum <= maximum, "Minimum value should be less than or equal to maximum!"
    return max(minimum, min(number, maximum))
