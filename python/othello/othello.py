#!/usr/bin/env python3
"""
Class Othello and main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019-2023
"""
import argparse
import hashlib
import sys

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
            if self.settings.show_log:
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

        print_bold("\nPlayers:")
        self._print_status()

    def _game_loop(self):
        """Keep making moves until both players can't make a move any more."""
        while self.board.can_play() and (self.player_black.can_play or self.player_white.can_play):
            self.rounds_played += 1
            print_bold(f"\n=========== ROUND: {self.rounds_played} ===========")
            for player in (self.player_black, self.player_white):
                result = player.play_one_move(self.board)
                if result:
                    self.game_log.append(f"{result};{self.board.to_log_entry()}")
                print("-------------------------------")

        self.games_played += 1

    def _print_log(self):
        """Print game log which shows all moves made and the game board state after each move."""
        formatted_log = "\n".join(
            f"{index:02}: {line}" for index, line in enumerate(self.game_log, start=1)
        )
        hex_hash = hashlib.sha256(formatted_log.encode()).hexdigest()

        print_bold("Game log:", Color.yellow)
        print(formatted_log)
        print(hex_hash)

    def _print_result(self):
        """Print ending status and winner info."""
        print_bold("\n===============================")
        print_bold("The game is finished!\n", Color.green)
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


if __name__ == "__main__":
    print_bold("OTHELLO GAME - PYTHON", Color.green)

    parser = argparse.ArgumentParser(
        prog="Othello Python",
        description="A simple Othello CLI game implementation.",
        # Specify custom help
        add_help=False,
    )
    # Optional positional argument for the board size
    parser.add_argument(
        "size",
        nargs="?",
        type=int,
        help=f"Optional board size ({MIN_BOARD_SIZE}..{MAX_BOARD_SIZE})",
    )
    parser.add_argument(
        "-h",
        "--help",
        action="help",
        default=argparse.SUPPRESS,
        help="Print help and exit",
    )

    exclusive = parser.add_mutually_exclusive_group()
    exclusive.add_argument(
        "-a",
        "--autoplay",
        action="store_true",
        help="Enable autoplay mode",
    )
    exclusive.add_argument(
        "-d",
        "--default",
        action="store_true",
        help="Play with default settings",
    )

    parser.add_argument(
        "-l",
        "--log",
        action="store_true",
        help="Show log after a game",
    )
    parser.add_argument(
        "-n",
        "--no-helpers",
        action="store_true",
        help="Hide disk placement hints",
    )
    parser.add_argument(
        "-t",
        "--test",
        action="store_true",
        help="Enable test mode",
    )
    parser.add_argument(
        "-v",
        "--version",
        action="version",
        version=f"Othello Python {version_info()}",
        help="Print version and exit",
    )

    args = parser.parse_args()

    try:
        # try to read board size from command line args
        if args.size is not None:
            board_size = args.size
            if board_size < MIN_BOARD_SIZE or board_size > MAX_BOARD_SIZE:
                print_error(f"Unsupported board size: {board_size}")
                sys.exit(1)

            print(f"Using board size: {board_size}")
        elif args.autoplay or args.default:
            board_size = DEFAULT_BOARD_SIZE
        else:
            # Otherwise ask user for board size
            board_size = Othello.get_board_size()

        settings = Settings(
            board_size=board_size,
            autoplay_mode=args.autoplay,
            use_defaults=args.default,
            show_helpers=not args.no_helpers,
            show_log=args.log,
            test_mode=args.test,
        )

        game = Othello(settings)
        game.play()
    except KeyboardInterrupt:
        # Catches CTRL-C
        sys.exit("\ncancelled...")
