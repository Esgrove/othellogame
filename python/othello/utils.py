"""
Utils
Helper utilities for Othello game
Akseli Lukkarila
2019-2025
"""

import hashlib
import sys

try:
    from othello.version import GIT_BRANCH, GIT_COMMIT, BUILD_TIME, VERSION_NUMBER
except ModuleNotFoundError:
    from version import GIT_BRANCH, GIT_COMMIT, BUILD_TIME, VERSION_NUMBER


def calculate_sha256(formatted_log: str) -> str:
    """Calculate SHA256 hash for the given string."""
    return hashlib.sha256(formatted_log.encode()).hexdigest()


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
    return f"{VERSION_NUMBER} {BUILD_TIME} {GIT_BRANCH} {GIT_COMMIT}"
