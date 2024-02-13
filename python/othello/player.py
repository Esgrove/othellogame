"""
Class Player
Defines one player for Othello
Akseli Lukkarila
2019-2024
"""

import random
import time
from typing import Self

try:
    from othello.board import Board
    from othello.colorprint import Color, print_color, print_error
    from othello.utils import Disk, Move, PlayerSettings, Square
except ModuleNotFoundError:
    from board import Board
    from colorprint import Color, print_color, print_error
    from utils import Disk, Move, PlayerSettings, Square


class Player:
    """Defines one player that can be either human or computer controlled."""

    def __init__(self, disk: Disk, settings=PlayerSettings.default(), is_human=True):
        self.can_play: bool = True
        self._disk: Disk = disk
        self._human: bool = is_human
        self._rounds_played: int = 0
        self._settings: PlayerSettings = settings

    @classmethod
    def black(cls, settings: PlayerSettings) -> Self:
        """Shorthand to initialize a new player for black disks."""
        return Player(Disk.BLACK, settings)

    @classmethod
    def white(cls, settings: PlayerSettings) -> Self:
        """Shorthand to initialize a new player for white disks."""
        return Player(Disk.WHITE, settings)

    def play_one_move(self, board: Board) -> str | None:
        """Play one round as this player."""
        print(f"Turn: {str(self._disk)}")
        moves = board.possible_moves(self._disk)
        if moves:
            self.can_play = True
            if self._human and self._settings.show_helpers:
                board.print_possible_moves(moves)

            chosen_move = (
                self._get_human_move(moves) if self._human else self._get_computer_move(moves)
            )
            board.place_disk(chosen_move)
            board.print_score()
            self._rounds_played += 1
            if not self._settings.test_mode:
                time.sleep(1)

            return chosen_move.to_log_entry()

        self.can_play = False
        print_color("  No moves available...", Color.yellow)
        return None

    def reset(self) -> None:
        """Reset player status for a new game."""
        self.can_play = True
        self._rounds_played = 0

    def set_human(self, is_human: bool) -> None:
        """Set the player as human or computer controlled."""
        self._human = is_human

    def _get_computer_move(self, moves: list[Move]) -> Move:
        """Return move chosen by computer."""
        print("  Computer plays...")
        if self._settings.test_mode:
            chosen_move = moves[0]
        else:
            # Wait a bit and pick a random move
            time.sleep(random.uniform(1.0, 2.0))
            chosen_move = random.choice(moves)

        print(f"  {chosen_move.square} -> {chosen_move.value}")
        return chosen_move

    def _get_human_move(self, moves: list[Move]) -> Move:
        """Return move chosen by a human player."""
        while True:
            square = self._get_square()
            # Check that the chosen square is actually one of the possible moves
            move = next((move for move in moves if move.square == square), None)
            if move:
                return move

            print_error(f"  Can't place a {self._disk} disk in square {square}!")

    @staticmethod
    def _get_square() -> Square:
        """Ask human player for square coordinates."""
        while True:
            try:
                pos = input("  Give disk position (x,y): ")
                x, y = (int(i) for i in pos.split(",") if i.strip())
                return Square(x, y)
            except ValueError:
                print_error("  Give coordinates in the form 'x,y'!")

    def _type_string(self) -> str:
        """Return player type description string."""
        return "Human   " if self._human else "Computer"

    def __str__(self) -> str:
        return f"{str(self._disk)} | {self._type_string()} | Moves: {self._rounds_played}"
