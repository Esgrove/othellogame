"""
Utils
Helper utilities for Othello game
Akseli Lukkarila
"""
from enum import Enum
#===========================================================

# board state enums
class Disk(Enum):
    BLACK = -1
    EMPTY = 0
    WHITE = 1

#===========================================================

class Move:
    """Represents one disk place position and the resulting value gained"""
    def __init__(self, x, y, value = 0):
        self.x = x
        self.y = y
        self.value = value

    # ------------------------------------------------------

    def __str__(self):
        return "point: ({},{}) -> value: {}".format(self.x, self.y, self.value)

    # ------------------------------------------------------

    def __eq__(self, other):
        return (self.x, self.y, self.value) == (other.x, other.y, other.value)

    def __ne__(self, other):
        return self.value != other.value

    def __lt__(self, other):
        return self.value < other.value or (self.value <= other.value and self.x <= other.x and self.y < other.y)

    def __le__(self, other):
        return self.value <= other.value and self.x <= other.x and self.y <= other.y

    def __gt__(self, other):
        return self.value > other.value or (self.value >= other.value and self.x >= other.x and self.y > other.y)

    def __ge__(self, other):
        return self.value >= other.value and self.x >= other.x and self.y >= other.y
