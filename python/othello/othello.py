#!/usr/bin/env python3
"""
Class Othello and main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019-2026
"""

import sys

import click

try:
    from othello.board import Board
    from othello.colorprint import (
        print_bold,
        print_error,
        print_green_bold,
        print_warn,
        print_yellow,
        print_yellow_bold,
    )
    from othello.models import Disk
    from othello.player import Player
    from othello.settings import DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE, Settings
    from othello.utils import calculate_sha256, clamp
    from othello.version import version_info
except ModuleNotFoundError:
    from board import Board
    from colorprint import (
        print_bold,
        print_error,
        print_green_bold,
        print_warn,
        print_yellow,
        print_yellow_bold,
    )
    from models import Disk
    from player import Player
    from settings import DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE, Settings
    from utils import calculate_sha256, clamp
    from version import version_info


class Othello:
    """Gameplay loop and main logic."""

    def __init__(self, settings: Settings):
        """Initialize Othello game."""
        self.board = Board(settings.board_size)
        self.settings = settings
        self.player_black = Player.black(settings.to_player_settings())
        self.player_white = Player.white(settings.to_player_settings())
        self.game_log: list[str] = []
        self.games_played = 0
        self.rounds_played = 0

    def play(self):
        """Play one full game of Othello."""
        while True:
            self._init_game()
            self._game_loop()
            self._print_result()
            if self.settings.show_log:
                self._print_log()

            if self.settings.autoplay_mode or not self.get_answer("Would you like to play again"):
                break

    def _init_game(self):
        """Initialize game board and players for a new game."""
        # Re-use existing objects instead of initializing new ones
        if self.games_played > 0:
            self.board = Board(self.settings.board_size)
            self.player_black.reset()
            self.player_white.reset()
            self.rounds_played = 0
            self.game_log.clear()

        if self.settings.autoplay_mode:
            # Computer plays both
            self.player_black.set_computer()
            self.player_white.set_computer()
        elif self.settings.use_defaults:
            # Default: play as black against white computer player
            self.player_white.set_computer()
        elif self.get_answer("Would you like to play against the computer"):
            if self.get_answer("Would you like to play as black or white", yes="b", no="w"):
                self.player_white.set_computer()
            else:
                self.player_black.set_computer()

        if not self.settings.check_mode:
            print_bold("\nPlayers:")
            self._print_status()

    def _game_loop(self):
        """Keep making moves until both players can't make a move any more."""
        while self.board.can_play() and (self.player_black.can_play or self.player_white.can_play):
            self.rounds_played += 1
            self._print_round_header()
            for player in (self.player_black, self.player_white):
                result = player.play_one_move(self.board)
                if result:
                    self.game_log.append(f"{result};{self.board.log_entry()}")

                if not self.settings.check_mode:
                    print("--------------------------------")

        self.games_played += 1
        self._print_game_end_footer()

    def _format_game_log(self) -> str:
        """Format game log with line numbers for each move."""
        return "\n".join(f"{index:02}: {line}" for index, line in enumerate(self.game_log, start=1))

    def _print_round_header(self):
        """Print header for the current round."""
        if not self.settings.check_mode:
            print_bold(f"\n=========== ROUND: {self.rounds_played} ===========")

    def _print_game_end_footer(self):
        """Print footer after the game has ended."""
        if not self.settings.check_mode:
            print_bold("\n================================")
            print_green_bold("The game is finished!\n")

    def _print_log(self):
        """Print game log which shows all moves made and the game board state after each move."""
        formatted_log = self._format_game_log()
        if not self.settings.check_mode:
            print_yellow_bold("Game log:")
            print(formatted_log)

        hex_hash = calculate_sha256(formatted_log)
        print(hex_hash)

    def _print_result(self):
        """Print ending status and winner info."""
        print_bold("Result:")
        self._print_status()
        print("")

        winner = self.board.result()
        if winner == Disk.EMPTY:
            print("The game ended in a tie...\n")
        else:
            print(f"The winner is {winner}!\n")

    def _print_status(self):
        """Print current board and player info."""
        print(self.player_black)
        print(self.player_white, end="\n\n")
        print(self.board)

    @staticmethod
    def get_answer(question: str, yes="y", no="n") -> bool:
        """Ask a question with two options, and return bool from user answer."""
        ans = input(f"{question} ({yes}/{no})? ")
        return ans.strip().lower() == yes.lower()

    @staticmethod
    def get_board_size() -> int:
        """Ask and return the desired board size."""
        try:
            ans = int(input(f"Choose board size (default is {DEFAULT_BOARD_SIZE}): ").strip())
            if ans < MIN_BOARD_SIZE or ans > MAX_BOARD_SIZE:
                print_yellow(
                    f"Limiting board size to valid range {MIN_BOARD_SIZE}..{MAX_BOARD_SIZE}"
                )
            return clamp(ans, MIN_BOARD_SIZE, MAX_BOARD_SIZE)
        except ValueError:
            print_warn(f"Invalid size, defaulting to {DEFAULT_BOARD_SIZE}...")

        return DEFAULT_BOARD_SIZE


@click.command(
    name="Othello",
    help=(
        "A simple Othello CLI game implementation in Python\n\n"
        f"[SIZE]  Optional board size ({MIN_BOARD_SIZE}..{MAX_BOARD_SIZE})"
    ),
    context_settings={"help_option_names": ["-h", "--help"]},
)
@click.argument(
    "size",
    required=False,
    # Could let click handle board size automatically,
    # but doing it manually to match other implementations.
    # type=click.IntRange(MIN_BOARD_SIZE, MAX_BOARD_SIZE, clamp=True)
    type=click.INT,
)
@click.option(
    "-a",
    "--autoplay",
    is_flag=True,
    help="Enable autoplay mode with computer control",
)
@click.option(
    "-c",
    "--check",
    is_flag=True,
    help="Autoplay and only print result",
)
@click.option(
    "-d",
    "--default",
    is_flag=True,
    help="Play with default settings",
)
@click.option(
    "-l",
    "--log",
    is_flag=True,
    help="Show game log at the end",
)
@click.option(
    "-n",
    "--no-helpers",
    is_flag=True,
    help="Hide disk placement hints",
)
@click.option(
    "-t",
    "--test",
    is_flag=True,
    help="Enable test mode with deterministic computer moves",
)
@click.option(
    "-v",
    "--version",
    is_flag=True,
    is_eager=True,
    help="Print version and exit",
)
def main(size, autoplay, check, default, log, no_helpers, test, version):
    # `-a` and `-d` are mutually exclusive (click does not support this natively)
    if autoplay and default:
        raise click.UsageError("'--autoplay' cannot be used with '--default'")

    if version:
        print(version_info())
        sys.exit(0)

    print_green_bold("OTHELLO GAME - PYTHON")
    try:
        board_size = resolve_board_size(size, autoplay, default)
        settings = Settings(
            board_size=board_size,
            autoplay_mode=autoplay or check,
            check_mode=check,
            show_helpers=not no_helpers,
            show_log=log or check,
            test_mode=test or check,
            use_defaults=default,
        )
        Othello(settings).play()
    except KeyboardInterrupt:
        # Catches CTRL-C
        sys.exit("\naborted...")


def resolve_board_size(size: int | None, autoplay: bool, default: bool) -> int:
    """Resolve the board size to use from CLI arguments, or by asking the user."""
    # Try to read board size from command line args
    if size is not None:
        if size < MIN_BOARD_SIZE or size > MAX_BOARD_SIZE:
            print_error(f"Unsupported board size: {size}")
            sys.exit(1)

        print(f"Using board size: {size}")
        return size
    elif autoplay or default:
        return DEFAULT_BOARD_SIZE
    else:
        # Otherwise ask user for board size
        return Othello.get_board_size()


if __name__ == "__main__":
    main()
