"""
Class Othello + main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019
"""
import sys

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

        if self.get_answer("Would you like to play against the computer"):
            if self.get_answer("Would you like to play as black or white", yes="b", no="w"):
                self.player_white._human = False
            else:
                self.player_black._human = False

        print_bold("\nPlayers:")
        print(self.player_black)
        print(self.player_white, end="\n\n")
        print(self.board)

    def play(self):
        """Play one full game of Othello."""
        self.init_game()
        self.play_loop()
        self.show_result()
        if self.get_answer("\nWould you like to play again"):
            self.play()

    def play_loop(self):
        while self.board.can_play() and (self.player_black.can_play() or self.player_white.can_play()):
            self.rounds_played += 1
            print_bold(f"\n=========== ROUND: {self.rounds_played} ===========")
            self.player_black.play_one_move(self.board)
            print("-------------------------------")
            self.player_white.play_one_move(self.board)

    def show_result(self):
        print_bold("===============================")
        print_bold("\nThe game is finished!", Color.green)
        print(f"total rounds played: {self.rounds_played}")
        print_bold("Result:")
        print(self.board, end="\n\n")
        print(self.player_black)
        print(self.player_white, end="\n\n")

        winner = self.board.result()
        if winner == Disk.EMPTY:
            print_bold("The game ended in a tie...")
        else:
            print_bold(f"The winner is {str(winner)}!")

    @staticmethod
    def get_answer(message: str, yes="y", no="n") -> bool:
        """Ask a question with two options, and return bool from user answer."""
        ans = input(f"{message} ({yes}/{no})? ")
        return ans.lower() == yes

    @staticmethod
    def get_board_size() -> int:
        """Ask and return the desired board size."""
        while True:
            try:
                size = int(input("Choose board size (default is 8): "))
                return clamp(size, 4, 8)
            except ValueError:
                print_error("Give a number...")
                continue


if __name__ == "__main__":
    print_bold("OTHELLO GAME - Python\n", Color.green)
    try:
        game = Othello()
        game.play()
    except KeyboardInterrupt:
        sys.exit("\nGame aborted...")
