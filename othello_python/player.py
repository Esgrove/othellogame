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
from util import Disk


class Player:
    """Defines one player."""
    def __init__(self, color: Disk, human=True):
        self.color = color
        self.human = human
        self.can_play = True
        self.rounds_played = 0

    def play_one_move(self, board: Board):
        """Play one round as this player."""
        print(f"Turn: {str(self.color)}")
        moves = board.get_possible_moves(self.color)
        if moves:
            self.can_play = True
            if self.human:
                self.print_possible_moves(moves)
                while True:
                    pos = input("  Give disk position (x,y): ")
                    square = [x.strip() for x in pos.split(",") if x.strip()]
                    if len(square) == 2:
                        try:
                            x = int(square[0])
                            y = int(square[1])
                            if board.place_disk(x, y, self.color):
                                break
                            else:
                                print_error(f"Can't place a {str(self.color)} disk in square ({x},{y})!")
                        except ValueError:
                            print_error("Coordinates have to be integer numbers.")
                    else:
                        print_error("Give coordinates in the form 'x,y'.")
            else:
                # computer plays: pick a random move
                print("  Computer plays...")
                time.sleep(random.uniform(1.0, 3.0))
                square = random.choice(moves).square
                print(f"  -> {square}")
                board.place_disk(square.x, square.y, self.color)

            self.rounds_played += 1
            print("\nResult:")
            print(board)

        else:
            print_color("  No moves available...", Color.red)
            self.can_play = False

        print("-------------------------------")

    @staticmethod
    def print_possible_moves(moves):
        print_color(f"  Possible plays ({len(moves)}):", Color.yellow)
        for move in sorted(moves):
            print(f"  {move}")

    def type_string(self):
        return "Human   " if self.human else "Computer"

    def __str__(self):
        return f"{str(self.color)} | {self.type_string()} | Moves: {self.rounds_played}"
