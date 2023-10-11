#!/usr/bin/env python3
"""
Class Othello and main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019-2023
"""
import sys

from board import Board
from colorprint import Color, print_bold, print_error, print_warn
from player import Player
from utils import DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE, Disk, clamp
from version import BRANCH, COMMIT, DATE, VERSION_NUMBER


class Othello:
    """Gameplay loop and main logic."""

    def __init__(self, board_size: int):
        self.board = Board(board_size)
        self.board_size = board_size
        self.player_black = Player.black()
        self.player_white = Player.white()
        self.rounds_played = 0
        self.games_played = 0

    def play(self):
        """Play one full game of Othello."""
        while True:
            self._init_game()
            self._game_loop()
            self._print_result()
            if not self.get_answer("\nWould you like to play again"):
                break

    def _init_game(self):
        """Initialize game board and players for a new game."""
        if self.games_played > 0:
            self.board = Board(self.board_size)
            self.player_black.reset()
            self.player_white.reset()
            self.rounds_played = 0

        if self.get_answer("Would you like to play against the computer"):
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
            self.player_black.play_one_move(self.board)
            print("-------------------------------")
            self.player_white.play_one_move(self.board)

        self.games_played += 1

    def _print_result(self):
        """Print ending status and winner info."""
        print_bold("\n===============================")
        print_bold("The game is finished!\n", Color.green)
        print_bold("Result:")
        self._print_status()
        print("")

        winner = self.board.result()
        if winner == Disk.EMPTY:
            print_bold("The game ended in a tie...")
        else:
            print_bold(f"The winner is {winner}!")

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
    args = sys.argv[1:]
    # Handle 'help' and 'version' arguments
    if "--help" in args or "-h" in args:
        sys.exit(
            f"Othello Python {VERSION_NUMBER} {DATE} {BRANCH} {COMMIT}\n\n"
            "USAGE: python3 othello.py [board size]\n\n"
            "Optional arguments:\n"
            "    -h | --help          Print usage and exit\n"
            "    -v | --version       Print version info and exit"
        )
    if "--version" in args or "-v" in args:
        print(f"Othello Python {VERSION_NUMBER} {DATE} {COMMIT} {BRANCH}")
        sys.exit(0)

    try:
        try:
            # try to read board size from command line args
            board_size = int(args[0])
            if board_size < MIN_BOARD_SIZE or board_size > MAX_BOARD_SIZE:
                print_error(f"Unsupported board size: {board_size}")
                raise ValueError

            print(f"Using board size: {board_size}")
        except (ValueError, IndexError):
            # Otherwise ask user for board size
            board_size = Othello.get_board_size()

        game = Othello(board_size)
        game.play()
    except KeyboardInterrupt:
        # Catches CTRL-C
        sys.exit("\naborted...")
