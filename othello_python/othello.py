"""
Class Othello + main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019
"""
from board import Board
from player import Player
from util import Disk, clamp

from colorprint import Color, print_bold, print_error


class Othello:
    """Implements Othello CLI game."""
    def __init__(self):
        self.board = None
        self.player_black = None
        self.player_white = None
        self.rounds_played = 0

    def init_game(self):
        """Initialize game board and players."""
        board_size = self.get_board_size()
        self.board = Board(board_size)
        self.player_black = Player(Disk.BLACK)
        self.player_white = Player(Disk.WHITE)
        self.rounds_played = 0

        play_against_computer = input("Would you like to play against the computer (y/n)? ")
        if play_against_computer.lower() == "y":
            side = input("Would you like to play as black or white (b/w)? ")
            if side.lower() == "b":
                self.player_white.human = False
            else:
                self.player_black.human = False

        print_bold("\nPlayers:")
        print(self.player_black)
        print(self.player_white, end="\n\n")
        print(self.board)

    def play_game(self):
        """Play one full game of Othello."""
        self.init_game()
        while self.player_black.can_play or self.player_white.can_play:
            print(f"=========== ROUND: {self.rounds_played + 1} ===========")
            self.player_black.play_one_move(self.board)
            self.player_white.play_one_move(self.board)
            self.board.print_score()
            self.rounds_played += 1

        print_bold("The game is finished!", Color.green)
        print(f"total rounds played: {self.rounds_played}\n")
        print_bold("Result:")
        print(self.board)
        print(self.player_black)
        print(self.player_white, end="\n\n")

        winner = self.board.get_result()
        if winner == Disk.WHITE:
            print_bold("The white player won!", Disk.WHITE.color())
        elif winner == Disk.BLACK:
            print_bold("The black player won!", Disk.BLACK.color())
        else:
            print_bold("The game ended in a tie...")

        if input("\nWould you like to play again (y/n)? ").lower() == "y":
            self.play_game()

    @staticmethod
    def get_board_size():
        """Ask the desired board size from user."""
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
