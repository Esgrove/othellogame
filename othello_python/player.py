"""
Class Player
Defines one player for Othello
Akseli Lukkarila
"""
import random

from util import Disk
from color import Color, printColor, printError
#===========================================================

class Player:
    def __init__(self, color, human = True):
        self.color = color
        self.human = human
        self.rounds = 0
        self.canPlay = True

    # ------------------------------------------------------

    def __str__(self):
        return "Player {} ({}) // moves: {}".format(self.color.name.upper(), "Human" if self.human else "Computer", self.rounds)

    # ------------------------------------------------------

    def playOneMove(self, board):
        moves = board.getPossibleMoves(self.color)
        if moves:
            self.canPlay = True
            if self.human:
                moves.sort(reverse=True)
                self.printMoves(moves)
                while True:
                    pos = input("  Give disk position (x,y): ")
                    square = pos.split(",")
                    if len(square) == 2:
                        try:
                            x = int(square[0])
                            y = int(square[1])
                            if board.placeDisk(x, y, self.color):
                                break
                            else:
                                printError("Can't place a {} disk in square ({},{}).".format(
                                    board.getDiskString(self.color), square[0], square[1]))
                        except ValueError:
                            printError("Coordinates have to be integer numbers.")
                    else:
                        printError("Give coordinates in the form x,y.")
            else:
                # computer plays: pick a random move
                print("  Computer plays...")
                move = random.choice(moves)
                board.placeDisk(move.x, move.y, self.color)

            self.rounds += 1
            print("\nResult:\n{}".format(board))

        else:
            print("  No moves available...")
            self.canPlay = False

        print("-----------------------------------")

    # ------------------------------------------------------

    def printMoves(self, moves):
        printColor("  Possible plays ({}):".format(len(moves)), Color.yellow)
        for move in moves:
            print(" ", move)
