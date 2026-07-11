import pytest

from othello.othello import Othello
from othello.settings import Settings
from othello.utils import calculate_sha256


def autoplay_settings(board_size: int = 8) -> Settings:
    """Deterministic autoplay settings that need no user input and print nothing."""
    return Settings(
        board_size=board_size,
        autoplay_mode=True,
        check_mode=True,
        show_helpers=False,
        show_log=True,
        test_mode=True,
        use_defaults=False,
    )


@pytest.fixture
def othello():
    return Othello(autoplay_settings())


def test_play_one_game(othello):
    othello._init_game()
    othello._game_loop()
    assert othello.games_played == 1
    assert othello.rounds_played > 0
    assert othello.game_log

    # Deterministic test mode: log hash must match the known value for board size 8
    assert (
        calculate_sha256(othello._format_game_log())
        == "2e9929fc0c2e6a2218dad6b7ad5e5c631708060bb4407d36e5cff6126181f05a"
    )


def test_game_log_cleared_on_replay(othello):
    """Regression test: replaying must reset the game log between games."""
    othello._init_game()
    othello._game_loop()
    first_log = list(othello.game_log)
    assert first_log

    othello._init_game()
    othello._game_loop()
    second_log = list(othello.game_log)
    assert othello.games_played == 2

    # If the log was not cleared, the second log would contain both games
    assert len(second_log) == len(first_log)
    # Test mode is deterministic so both games produce an identical log
    assert second_log == first_log


def test_format_game_log_line_numbers(othello):
    othello.game_log = ["B:(0,1),1;____BBB__BW_____", "W:(0,0),1;W___BWB__BW_____"]
    assert othello._format_game_log() == (
        "01: B:(0,1),1;____BBB__BW_____\n02: W:(0,0),1;W___BWB__BW_____"
    )
