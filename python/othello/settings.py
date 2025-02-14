from typing import Self


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
