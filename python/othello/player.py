"""
Class Player
Defines one player for Othello
Akseli Lukkarila
2019-2026
"""

from __future__ import annotations

import random
import time

from enum import IntEnum
from typing import Self

try:
    from othello.board import Board
    from othello.colorprint import Color, print_color, print_error
    from othello.models import Disk, Move, Square
    from othello.settings import PlayerSettings
except ModuleNotFoundError:
    from board import Board
    from colorprint import Color, print_color, print_error
    from models import Disk, Move, Square
    from settings import PlayerSettings


class Player:
    """Defines one player that can be either human or computer controlled."""

    def __init__(self, disk: Disk, settings: PlayerSettings | None = None):
        """Initialize new player for the given disk color."""
        self.can_play: bool = True
        self._disk: Disk = disk
        self._player_type: PlayerType = PlayerType.HUMAN
        self._rounds_played: int = 0
        self._settings: PlayerSettings = (
            settings if settings is not None else PlayerSettings.default()
        )

    @classmethod
    def black(cls, settings: PlayerSettings) -> Self:
        """Shorthand to initialize a new player for black disks."""
        return cls(Disk.BLACK, settings)

    @classmethod
    def white(cls, settings: PlayerSettings) -> Self:
        """Shorthand to initialize a new player for white disks."""
        return cls(Disk.WHITE, settings)

    def play_one_move(self, board: Board) -> str | None:
        """Play one round as this player."""
        if not self._settings.check_mode:
            print(f"Turn: {self._disk}")

        moves = board.possible_moves(self._disk)
        if not moves:
            self.can_play = False
            if not self._settings.check_mode:
                print_color("  No moves available...", Color.yellow)

            return None

        self.can_play = True
        if self.human() and self._settings.show_helpers and not self._settings.check_mode:
            board.print_possible_moves(moves)

        chosen_move = (
            self._get_human_move(moves) if self.human() else self._get_computer_move(moves)
        )
        board.place_disk(chosen_move)
        if not self._settings.check_mode:
            board.print_score()
        self._rounds_played += 1
        if not self._settings.test_mode:
            time.sleep(1)

        return chosen_move.log_entry()

    def reset(self) -> None:
        """Reset player status for a new game."""
        self.can_play = True
        self._rounds_played = 0

    def human(self) -> bool:
        """Returns true if player is controlled by a human player."""
        return self._player_type.human()

    def computer(self) -> bool:
        """Returns true if player is controlled by computer."""
        return self._player_type.computer()

    def set_player_type(self, player_type: PlayerType) -> None:
        """Set the player as human or computer controlled."""
        self._player_type = player_type

    def set_human(self) -> None:
        """Set the player as human controlled."""
        self.set_player_type(PlayerType.HUMAN)

    def set_computer(self) -> None:
        """Set the player as computer controlled."""
        self.set_player_type(PlayerType.COMPUTER)

    def _get_computer_move(self, moves: list[Move]) -> Move:
        """Return move chosen by computer."""
        if not self._settings.check_mode:
            print("  Computer plays...")

        if self._settings.test_mode:
            chosen_move = moves[0]
        else:
            # Wait a bit and pick a random move
            time.sleep(random.uniform(1.0, 2.0))
            chosen_move = random.choice(moves)

        if not self._settings.check_mode:
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
            pos = input("  Give disk position (x,y): ")
            values = pos.strip().split(",")
            if len(values) == 2:
                try:
                    x = int(values[0])
                    y = int(values[1])
                except ValueError:
                    x, y = -1, -1
                if x >= 0 and y >= 0:
                    return Square(x, y)

            print_error("  Give coordinates in the form 'x,y'!")

    def type_string(self) -> str:
        """Return player type description string."""
        return str(self._player_type)

    def __str__(self) -> str:
        return f"{self._disk} | {self.type_string()} | Moves: {self._rounds_played}"


class PlayerType(IntEnum):
    """Player can be controlled either by a human or computer."""

    HUMAN = 0
    COMPUTER = 1

    def human(self) -> bool:
        """Check if the player is controlled by a human."""
        return self == PlayerType.HUMAN

    def computer(self) -> bool:
        """Check if the player is controlled by a computer."""
        return self == PlayerType.COMPUTER

    def __str__(self) -> str:
        return "Human   " if self == PlayerType.HUMAN else "Computer"
