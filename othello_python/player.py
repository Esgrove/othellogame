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
from util import Disk, Move, Square


class Player:
    """Defines one player."""
    def __init__(self, color: Disk, human=True, show_helpers=True):
        self._color = color
        self._human = human
        self._show_helpers = show_helpers
        self._rounds_played = 0
        self._can_play = True

    def can_play(self):
        return self._can_play

    def play_one_move(self, board: Board):
        """Play one round as this player."""
        print(f"Turn: {str(self._color)}")
        moves = board.possible_moves(self._color)
        if moves:
            self._can_play = True
            if self._human and self._show_helpers:
                board.print_possible_moves(moves)

            chosen_move = self.get_human_move(moves) if self._human else self.get_computer_move(moves)
            board.place_disk(chosen_move.square.x, chosen_move.square.y, self._color, chosen_move.directions)
            board.print_score()
            self._rounds_played += 1
        else:
            self._can_play = False
            print_color("  No moves available...", Color.yellow)

    @staticmethod
    def get_computer_move(moves: List[Move]) -> Move:
        """Return move chosen by computer."""
        print("  Computer plays...")
        # wait a bit and pick a random move
        time.sleep(random.uniform(1.0, 2.0))
        move = random.choice(moves)
        print(f"  -> {move.square}")
        return move

    def get_human_move(self, moves: List[Move]) -> Move:
        """Return move chosen by a human player."""
        while True:
            square = self.get_square()
            if not square:
                continue
            # check if given square is one of the possible moves
            move = next((move for move in moves if move.square == square), None)
            if move:
                return move
            else:
                print_error(f"Can't place a {str(self._color)} disk in square {square}!")

    @staticmethod
    def get_square():
        """Ask human player for square coordinates."""
        try:
            pos = input("  Give disk position (x,y): ")
            coordinates = [int(x.strip()) for x in pos.split(",") if x.strip()]
            if not len(coordinates) == 2:
                raise ValueError
            return Square(coordinates[0], coordinates[1])
        except ValueError:
            print_error("Give coordinates in the form 'x,y'!")
            return None

    def type_string(self):
        return "Human   " if self._human else "Computer"

    def set_player_type(self, is_human: bool):
        """Set player to be controlled by human or computer."""
        self._human = is_human

    def __str__(self):
        return f"{str(self._color)} | {self.type_string()} | Moves: {self._rounds_played}"
