"""
Main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
"""
from board import Board
from player import Player
from util import Disk, clamp

from colorprint import Color, print_bold, print_error


class Othello:
    def __init__(self):
        self.board = None
        self.player_1 = None
        self.player_2 = None

    def init_game(self):
        board_size = self.get_board_size()
        self.board = Board(board_size)
        self.player_1 = Player(Disk.BLACK)
        self.player_2 = Player(Disk.WHITE)

        play_against_computer = input("Would you like to play against the computer (y/n)? ")
        if play_against_computer.lower() == "y":
            side = input("Would you like to play as black or white (b/w)? ")
            if side.lower() == "b":
                self.player_2.human = False
            else:
                self.player_1.human = False

        print_bold("\nPlayers:")
        print(self.player_1)
        print(self.player_2, end="\n\n")
        print(self.board)

    def play_game(self):
        self.init_game()
        while self.player_1.can_play or self.player_2.can_play:
            self.player_1.play_one_move(self.board)
            self.player_2.play_one_move(self.board)
            self.board.print_score()

        print_bold("The game is finished!", Color.green)
        print_bold("Result:")
        print(self.board)
        print(self.player_1)
        print(self.player_2)
        print()

        winner = self.board.get_result()
        if winner == Disk.WHITE:
            print_bold("The white player won!", Color.magenta)
        elif winner == Disk.BLACK:
            print_bold("The black player won!", Color.cyan)
        else:
            print_bold("The game ended in a tie...")

        answer = input("Would you like to play again (y/n)? ")
        if answer.lower() == "y":
            self.play_game()

    @staticmethod
    def get_board_size():
        while True:
            try:
                size = int(input("Choose board size (default is 8): "))
                return clamp(size, 4, 8)
            except ValueError:
                print_error("Give a number...")
                continue


if __name__ == "__main__":
    print_bold("OTHELLO GAME - Python\n", Color.green)
    game = Othello()
    game.play_game()
