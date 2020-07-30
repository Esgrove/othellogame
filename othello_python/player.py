"""
Class Player
Defines one player for Othello
Akseli Lukkarila
2019
"""
import random
import time

from board import Board
from colorprint import Color, print_color, print_error
from util import Disk, Square


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
            if self._human:
                if self._show_helpers:
                    board.print_possible_moves(moves)
                while True:
                    pos = input("  Give disk position (x,y): ")
                    try:
                        coordinates = [int(x.strip()) for x in pos.split(",") if x.strip()]
                        if not len(coordinates) == 2:
                            raise ValueError
                        x, y = coordinates
                        square = Square(x, y)
                        # check if given square is one of the possible moves
                        move = next((move for move in moves if move.square == square), None)
                        if move:
                            board.place_disk(x, y, self._color, move.directions)
                            break
                        else:
                            print_error(f"Can't place a {str(self._color)} disk in square {square}!")
                    except ValueError:
                        print_error("Give coordinates in the form 'x,y'!")
            else:
                # computer plays: wait a bit and pick a random move
                print("  Computer plays...")
                time.sleep(random.uniform(1.0, 2.0))
                move = random.choice(moves)
                print(f"  -> {move.square}")
                board.place_disk(move.square.x, move.square.y, self._color, move.directions)

            self._rounds_played += 1
            print("")
            print(board)
            board.print_score()
        else:
            print_color("  No moves available...", Color.yellow)
            self._can_play = False

    def type_string(self):
        return "Human   " if self._human else "Computer"

    def __str__(self):
        return f"{str(self._color)} | {self.type_string()} | Moves: {self._rounds_played}"
