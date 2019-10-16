"""
Helper utilities for Othello game
Akseli Lukkarila
2019
"""
from enum import IntEnum

from colorprint import get_color, Color


class Disk(IntEnum):
    """Board state enums."""
    BLACK = -1
    EMPTY = 0
    WHITE = 1

    def other_disk(self):
        """Returns the opposing color."""
        if self.value == self.EMPTY:
            return self.EMPTY

        return self.WHITE if self.value == self.BLACK else self.BLACK

    def __str__(self):
        colors = {self.BLACK: Color.cyan, self.WHITE: Color.magenta, self.EMPTY: Color.white}
        return get_color(self.name, colors[self.value])

    def __repr__(self):
        if self.value == self.EMPTY:
            return "_"

        return get_color(self.name[0], Color.cyan if self.value == self.BLACK else Color.magenta)


class Move:
    """Represents one disk place position and the resulting value gained."""
    def __init__(self, x, y, value=0):
        self.x = x
        self.y = y
        self.value = value

    def __repr__(self):
        return f"point: ({self.x},{self.y}) -> value: {self.value}"

    def __str__(self):
        return f"({self.x},{self.y})"

    def __eq__(self, other):
        return (self.x, self.y, self.value) == (other.x, other.y, other.value)

    def __ne__(self, other):
        return (self.x, self.y, self.value) != (other.x, other.y, other.value)

    def __lt__(self, other):
        return self.value > other.value or (self.value <= other.value and self.x <= other.x and self.y < other.y)

    def __le__(self, other):
        return self.value >= other.value and self.x <= other.x and self.y <= other.y

    def __gt__(self, other):
        return self.value < other.value or (self.value >= other.value and self.x >= other.x and self.y > other.y)

    def __ge__(self, other):
        return self.value <= other.value and self.x >= other.x and self.y >= other.y


def get_board_string(disk: Disk):
    """Returns the identifier string for the given disk."""
    return "_" if disk == Disk.EMPTY else str(disk)[0]


def clamp(number, minimum, maximum):
    """Clamp value to given range."""
    return max(minimum, min(number, maximum))
