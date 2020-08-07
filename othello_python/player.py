"""
Class Player
Defines one player for Othello
Akseli Lukkarila
2019
"""
import random
import time

from typing import List

from board import Board
from colorprint import Color, print_color, print_error
from utils import Disk, Move, Square


class Player:
    """Defines one player."""
    def __init__(self, disk: Disk, human=True, show_helpers=True):
        self._can_play = True
        self._disk = disk
        self._human = human
        self._rounds_played = 0
        self._show_helpers = show_helpers

    def can_play(self) -> bool:
        return self._can_play

    def play_one_move(self, board: Board) -> None:
        """Play one round as this player."""
        print(f"Turn: {str(self._disk)}")
        moves = board.possible_moves(self._disk)
        if moves:
            self._can_play = True
            if self._human and self._show_helpers:
                board.print_moves(moves)

            chosen_move = self._get_human_move(moves) if self._human else self._get_computer_move(moves)
            board.place_disk(chosen_move)
            board.print_score()
            self._rounds_played += 1
        else:
            self._can_play = False
            print_color("  No moves available...", Color.yellow)

    def set_human(self, is_human: bool):
        """Set player to be controlled by human or computer."""
        self._human = is_human

    @staticmethod
    def _get_computer_move(moves: List[Move]) -> Move:
        """Return move chosen by computer."""
        print("  Computer plays...")
        # wait a bit and pick a random move
        time.sleep(random.uniform(1.0, 2.0))
        move = random.choice(moves)
        print(f"  -> {move.square}")
        return move

    def _get_human_move(self, moves: List[Move]) -> Move:
        """Return move chosen by a human player."""
        while True:
            square = self._get_square()
            # check if given square is one of the possible moves
            move = next((move for move in moves if move.square == square), None)
            if move:
                return move

            print_error(f"can't place a {str(self._disk)} disk in square {square}!", indent=2)

    @staticmethod
    def _get_square() -> Square:
        """Ask human player for square coordinates."""
        while True:
            try:
                pos = input("  Give disk position (x,y): ")
                coordinates = [int(x.strip()) for x in pos.split(",") if x.strip()]
                if len(coordinates) != 2:
                    raise ValueError
                return Square(coordinates[0], coordinates[1])
            except ValueError:
                print_error("give coordinates in the form 'x,y'!", indent=2)

    def _type_string(self) -> str:
        return "Human   " if self._human else "Computer"

    def __str__(self):
        return f"{str(self._disk)} | {self._type_string()} | Moves: {self._rounds_played}"
