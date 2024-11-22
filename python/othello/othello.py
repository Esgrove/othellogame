#!/usr/bin/env python3
"""
Class Othello and main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019-2024
"""

import hashlib
import sys

import click

try:
    from othello.board import Board
    from othello.colorprint import Color, print_bold, print_error, print_warn
    from othello.player import Player
    from othello.utils import (
        DEFAULT_BOARD_SIZE,
        MAX_BOARD_SIZE,
        MIN_BOARD_SIZE,
        Disk,
        Settings,
        clamp,
        version_info,
    )
except ModuleNotFoundError:
    from board import Board
    from colorprint import Color, print_bold, print_error, print_warn
    from player import Player
    from utils import (
        DEFAULT_BOARD_SIZE,
        MAX_BOARD_SIZE,
        MIN_BOARD_SIZE,
        Disk,
        Settings,
        clamp,
        version_info,
    )


class Othello:
    """Gameplay loop and main logic."""

    def __init__(self, settings: Settings):
        self.board = Board(settings.board_size)
        self.settings = settings
        self.player_black = Player.black(settings.to_player_settings())
        self.player_white = Player.white(settings.to_player_settings())
        self.rounds_played = 0
        self.games_played = 0
        self.game_log = []

    def play(self):
        """Play one full game of Othello."""
        while True:
            self._init_game()
            self._game_loop()
            self._print_result()
            if self.settings.show_log or self.settings.check_mode:
                self._print_log()

            if self.settings.autoplay_mode or not self.get_answer("Would you like to play again"):
                break

    def _init_game(self):
        """Initialize game board and players for a new game."""
        if self.games_played > 0:
            self.board = Board(self.settings.board_size)
            self.player_black.reset()
            self.player_white.reset()
            self.rounds_played = 0

        if self.settings.autoplay_mode:
            # Computer plays both
            self.player_white.set_human(False)
            self.player_black.set_human(False)
        elif self.settings.use_defaults:
            # Default: play as black against white computer player
            self.player_white.set_human(False)
        elif self.get_answer("Would you like to play against the computer"):
            if self.get_answer("Would you like to play as black or white", yes="b", no="w"):
                self.player_white.set_human(False)
            else:
                self.player_black.set_human(False)

        if not self.settings.check_mode:
            print_bold("\nPlayers:")
            self._print_status()

    def _game_loop(self):
        """Keep making moves until both players can't make a move any more."""
        while self.board.can_play() and (self.player_black.can_play or self.player_white.can_play):
            self.rounds_played += 1
            if not self.settings.check_mode:
                print_bold(f"\n=========== ROUND: {self.rounds_played} ===========")

            for player in (self.player_black, self.player_white):
                result = player.play_one_move(self.board)
                if result:
                    self.game_log.append(f"{result};{self.board.log_entry()}")

                if not self.settings.check_mode:
                    print("--------------------------------")

        self.games_played += 1
        if not self.settings.check_mode:
            print_bold("\n================================")
            print_bold("The game is finished!\n", Color.green)

    def _print_log(self):
        """Print game log which shows all moves made and the game board state after each move."""
        formatted_log = "\n".join(
            f"{index:02}: {line}" for index, line in enumerate(self.game_log, start=1)
        )
        hex_hash = hashlib.sha256(formatted_log.encode()).hexdigest()

        if not self.settings.check_mode:
            print_bold("Game log:", Color.yellow)
            print(formatted_log)

        print(hex_hash)

    def _print_result(self):
        """Print ending status and winner info."""
        print_bold("Result:")
        self._print_status()
        print("")

        winner = self.board.result()
        if winner == Disk.EMPTY:
            print_bold("The game ended in a tie...\n")
        else:
            print_bold(f"The winner is {winner}!\n")

    def _print_status(self):
        """Print current board and player info."""
        print(self.player_black)
        print(self.player_white, end="\n\n")
        print(self.board)

    @staticmethod
    def get_answer(question: str, yes="y", no="n") -> bool:
        """Ask a question with two options, and return bool from user answer."""
        ans = input(f"{question} ({yes}/{no})? ")
        return ans.lower() == yes.lower()

    @staticmethod
    def get_board_size() -> int:
        """Ask and return the desired board size."""
        try:
            ans = int(input(f"Choose board size (default is {DEFAULT_BOARD_SIZE}): "))
            if ans < MIN_BOARD_SIZE or ans > MAX_BOARD_SIZE:
                print_warn(
                    f"Limiting board size to valid range {MIN_BOARD_SIZE}...{MAX_BOARD_SIZE}"
                )
            return clamp(ans, MIN_BOARD_SIZE, MAX_BOARD_SIZE)
        except ValueError:
            print_warn(f"Invalid size, defaulting to {DEFAULT_BOARD_SIZE}...")

        return DEFAULT_BOARD_SIZE


@click.command(
    name="Othello",
    help="A simple Othello CLI game implementation in Python",
    context_settings={"help_option_names": ["-h", "--help"]},
)
@click.argument(
    "size",
    required=False,
    # Could let click handle board size automatically,
    # but doing it manually to match other implementations.
    # type=click.IntRange(MIN_BOARD_SIZE, MAX_BOARD_SIZE, clamp=True),
    type=click.INT,
)
@click.option(
    "-a",
    "--autoplay",
    is_flag=True,
    help="Enable autoplay mode",
)
@click.option(
    "-c",
    "--check",
    is_flag=True,
    help="Only print hash to check the result",
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
    help="Show log after a game",
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
    if version:
        print(f"Othello Python {version_info()}")
        sys.exit(0)

    print_bold("OTHELLO GAME - PYTHON", Color.green)
    try:
        # try to read board size from command line args
        if size is not None:
            board_size = size
            if board_size < MIN_BOARD_SIZE or board_size > MAX_BOARD_SIZE:
                print_error(f"Unsupported board size: {board_size}")
                sys.exit(1)

            print(f"Using board size: {board_size}")
        elif autoplay or default:
            board_size = DEFAULT_BOARD_SIZE
        else:
            # Otherwise ask user for board size
            board_size = Othello.get_board_size()

        settings = Settings(
            board_size=board_size,
            autoplay_mode=autoplay or check,
            check_mode=check,
            show_helpers=not no_helpers,
            show_log=log,
            test_mode=test or check,
            use_defaults=default,
        )

        Othello(settings).play()
    except KeyboardInterrupt:
        # Catches CTRL-C
        sys.exit("\naborted...")


if __name__ == "__main__":
    main()
