"""
Settings
Defines game settings
Akseli Lukkarila
2019-2026
"""

from __future__ import annotations

from typing import Self

MIN_BOARD_SIZE: int = 4
MAX_BOARD_SIZE: int = 10
DEFAULT_BOARD_SIZE: int = 8


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
        return cls(
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

    def __str__(self) -> str:
        # Note: bools are formatted lowercase to match the Rust implementation output
        return (
            "Settings:\n"
            f"  board_size: {self.board_size}\n"
            f"  autoplay: {str(self.autoplay_mode).lower()}\n"
            f"  check_mode: {str(self.check_mode).lower()}\n"
            f"  use_defaults: {str(self.use_defaults).lower()}\n"
            f"  show_helpers: {str(self.show_helpers).lower()}\n"
            f"  show_log: {str(self.show_log).lower()}\n"
            f"  test_mode: {str(self.test_mode).lower()}"
        )


class PlayerSettings:
    """Player settings."""

    def __init__(self, show_helpers: bool, check_mode: bool, test_mode: bool):
        self.show_helpers: bool = show_helpers
        self.check_mode: bool = check_mode
        self.test_mode: bool = test_mode

    @classmethod
    def default(cls) -> Self:
        return cls(show_helpers=True, check_mode=False, test_mode=False)

    def __eq__(self, other) -> bool:
        if isinstance(other, PlayerSettings):
            return (
                self.show_helpers == other.show_helpers
                and self.check_mode == other.check_mode
                and self.test_mode == other.test_mode
            )
        return NotImplemented
