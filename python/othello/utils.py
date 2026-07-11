"""
Utils
Helper utilities
Akseli Lukkarila
2019-2026
"""

import hashlib


def calculate_sha256(formatted_log: str) -> str:
    """Calculate SHA256 hash for the given string."""
    return hashlib.sha256(formatted_log.encode()).hexdigest()


def clamp(value: float, minimum: float, maximum: float) -> int | float:
    """Clamp value to given range."""
    # Note: `float` type hint accepts `int` as well:
    # # https://peps.python.org/pep-0484/#the-numeric-tower
    assert minimum <= maximum, "Minimum value should be less than or equal to maximum!"
    return max(minimum, min(value, maximum))
