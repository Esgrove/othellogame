"""
Class Player
Defines one player for Othello
Akseli Lukkarila
"""
import random
import time

from util import Disk
from colorprint import Color, print_color, print_error


class Player:
    def __init__(self, color: Disk, human=True):
        self.color = color
        self.human = human
        self.rounds = 0
        self.can_play = True

    def play_one_move(self, board):
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
                        print_error("Give coordinates in the form x,y.")
            else:
                # computer plays: pick a random move
                print("  Computer plays...")
                time.sleep(random.uniform(1.0, 3.0))
                move = random.choice(moves)
                print(f"  -> {str(move)}")
                board.place_disk(move.x, move.y, self.color)

            self.rounds += 1
            print("\nResult:\n{}".format(board))

        else:
            print("  No moves available...")
            self.can_play = False

        print("-----------------------------------")

    @staticmethod
    def print_possible_moves(moves):
        print_color(f"  Possible plays ({len(moves)}):", Color.yellow)
        for move in sorted(moves):
            print(f"  {repr(move)}")

    def __str__(self):
        return f"{str(self.color)} | {'Human' if self.human else 'Computer':8s} | Moves: {self.rounds}"
