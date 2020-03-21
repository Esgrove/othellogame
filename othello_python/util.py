"""
Helper utilities for Othello game
Akseli Lukkarila
2019
"""
from enum import IntEnum

from colorprint import get_color, Color


class Disk(IntEnum):
    """Board state enums and utility methods."""
    BLACK = -1
    EMPTY = 0
    WHITE = 1

    def color(self) -> Color:
        """Return the associated color for this disk."""
        if self.value == self.EMPTY:
            return Color.white

        return Color.magenta if self.value == self.BLACK else Color.cyan

    def board_char(self) -> str:
        """Returns the single character identifier string for the given disk."""
        if self.value == self.EMPTY:
            return "_"

        return get_color(self.name[0], self.color())

    def other_disk(self):
        """Returns the opposing disk."""
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
        """Enable iteration so coordinates can be unpacked: x,y = square"""
        if key == 0:
            return self.x
        elif key == 1:
            return self.y

        raise IndexError

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
    """Represents one disk place position and the resulting value gained."""
    def __init__(self, square: Square, value=0):
        self.square = square
        self.value = value

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
        return f"point: {self.square} -> value: {self.value}"


def clamp(number, minimum, maximum):
    """Clamp value to given range."""
    assert minimum <= maximum, "Minimum value should be less than or equal to maximum!"
    return max(minimum, min(number, maximum))
