"""
Utils
Helper utilities for Othello game
Akseli Lukkarila
2019-2025
"""

import sys
from enum import IntEnum

# Python 3.11+
from typing import Self

try:
    from othello.colorprint import Color, get_color
    from othello.version import BRANCH, COMMIT, DATE, VERSION_NUMBER
except ModuleNotFoundError:
    from colorprint import Color, get_color
    from version import BRANCH, COMMIT, DATE, VERSION_NUMBER

MIN_BOARD_SIZE: int = 4
MAX_BOARD_SIZE: int = 10
DEFAULT_BOARD_SIZE: int = 8


class Disk(IntEnum):
    """Represents one game piece or lack of one."""

    BLACK = -1
    EMPTY = 0
    WHITE = 1

    def board_char(self, color=True) -> str:
        """Returns a single character identifier string for the given disk."""
        if self.value == self.EMPTY:
            return "_"

        return get_color(self.name[0], self.color()) if color else self.name[0]

    def color(self) -> Color:
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

    def opponent(self) -> Self:
        """Return the opposing disk colour for this disk."""
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
    """Represents a step direction on the board."""

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
            # another way: `(self.x, self.y) == (other.x, other.y)`
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
        # `match` does not work here without the unpacking step :(
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
        if isinstance(other, Square):
            return self.x == other.x and self.y == other.y
        elif len(other) == 2:
            return self.x == other[0] and self.y == other[1]
        return NotImplemented

    def __ne__(self, other) -> bool:
        if isinstance(other, Square):
            return self.x != other.x or self.y != other.y
        elif len(other) == 2:
            return self.x != other[0] or self.y != other[1]
        return NotImplemented

    def __lt__(self, other) -> bool:
        if isinstance(other, Square):
            return self.x < other.x or (self.x <= other.x and self.y < other.y)
        elif len(other) == 2:
            return self.x < other[0] or (self.x <= other[0] and self.y < other[1])
        return NotImplemented

    def __le__(self, other) -> bool:
        if isinstance(other, Square):
            return self.x <= other.x and self.y < other.y
        elif len(other) == 2:
            return self.x <= other[0] and self.y < other[1]
        return NotImplemented

    def __gt__(self, other) -> bool:
        if isinstance(other, Square):
            return self.x > other.x or (self.x >= other.x and self.y > other.y)
        elif len(other) == 2:
            return self.x > other[0] or (self.x >= other[0] and self.y > other[1])
        return NotImplemented

    def __ge__(self, other) -> bool:
        if isinstance(other, Square):
            return self.x >= other.x and self.y >= other.y
        elif len(other) == 2:
            return self.x >= other[0] and self.y >= other[1]
        return NotImplemented

    def __str__(self) -> str:
        return f"({self.x},{self.y})"


class Move:
    """Represents one possible disk placement for the given disk color."""

    def __init__(self, square: Square, disk=Disk.EMPTY, value=0, directions=None):
        self.square: Square = square
        self.disk: Disk = disk
        self.value: int = value
        self.directions: list[tuple[Step, int]] = directions

    def log_entry(self) -> str:
        """Format move for log entry."""
        return f"{self.disk.board_char(color=False)}:{self.square},{self.value}"

    def affected_squares(self) -> list[Square]:
        """Get all the squares playing this move will change."""
        paths = []
        for step, count in self.directions:
            pos = self.square + step
            for _ in range(count):
                paths.append(pos)
                pos += step

        return sorted(paths)

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


class PlayerSettings:
    """Player settings."""

    def __init__(self, show_helpers: bool, check_mode: bool, test_mode: bool):
        self.show_helpers: bool = show_helpers
        self.check_mode: bool = check_mode
        self.test_mode: bool = test_mode

    @classmethod
    def default(cls) -> Self:
        return PlayerSettings(show_helpers=True, check_mode=False, test_mode=False)


class Settings:
    """Game settings."""

    def __init__(
        self,
        board_size: int,
        autoplay_mode: bool,
        check_mode: bool,
        show_helpers: bool,
        show_log: bool,
        test_mode: bool,
        use_defaults: bool,
    ):
        self.board_size: int = board_size
        self.autoplay_mode: bool = autoplay_mode
        self.check_mode: bool = check_mode
        self.show_helpers: bool = show_helpers
        self.show_log: bool = show_log
        self.test_mode: bool = test_mode
        self.use_defaults: bool = use_defaults

    @classmethod
    def default(cls) -> Self:
        return Settings(
            board_size=8,
            autoplay_mode=False,
            check_mode=False,
            show_helpers=True,
            show_log=False,
            test_mode=False,
            use_defaults=False,
        )

    def to_player_settings(self) -> PlayerSettings:
        """Get player setting values from overall game settings."""
        return PlayerSettings(
            show_helpers=self.show_helpers, check_mode=self.check_mode, test_mode=self.test_mode
        )


def clamp(value: float, minimum: float, maximum: float) -> int | float:
    """Clamp value to given range."""
    # Note: `float` type hint accepts `int` as well:
    # # https://peps.python.org/pep-0484/#the-numeric-tower
    assert minimum <= maximum, "Minimum value should be less than or equal to maximum!"
    return max(minimum, min(value, maximum))


def python_version() -> str:
    """
    Get the current Python version as a formatted string.
    For example: 3.11.5
    """
    return f"{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}"


def version_info() -> str:
    """Return formatted build version information."""
    return f"{VERSION_NUMBER} {DATE} {BRANCH} {COMMIT}"
