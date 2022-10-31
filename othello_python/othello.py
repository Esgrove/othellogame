"""
Class Othello + main
Play Othello (Reversi) on the command line
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019
"""
import sys

from board import Board
from colorprint import Color, print_bold, print_error
from player import Player
from utils import Disk, clamp


class Othello:
    """Play Othello CLI game."""

    def __init__(self, board_size: int):
        self.board = None
        self.player_black = None
        self.player_white = None
        self.rounds_played = 0
        self.size = board_size

    def init_game(self):
        """Initialize game board and players for a new game."""
        self.board = Board(self.size)
        self.player_black = Player(Disk.BLACK)
        self.player_white = Player(Disk.WHITE)
        self.rounds_played = 0

        if self.get_answer("Would you like to play against the computer"):
            if self.get_answer("Would you like to play as black or white", yes="b", no="w"):
                self.player_white.set_human(False)
            else:
                self.player_black.set_human(False)

        print_bold("\nPlayers:")
        self.print_status()

    def play(self):
        """Play one full game of Othello."""
        self.init_game()
        self.game_loop()
        self.print_result()
        if self.get_answer("\nWould you like to play again"):
            self.play()

    def game_loop(self):
        """Keep making moves until both players can't make a move anymore."""
        while self.board.can_play() and (self.player_black.can_play() or self.player_white.can_play()):
            self.rounds_played += 1
            print_bold(f"\n=========== ROUND: {self.rounds_played} ===========")
            self.player_black.play_one_move(self.board)
            print("-------------------------------")
            self.player_white.play_one_move(self.board)

    def print_result(self):
        """Print ending status and winner info."""
        print_bold("\n===============================")
        print_bold("The game is finished!\n", Color.green)
        print_bold("Result:")
        self.print_status()
        print("")

        winner = self.board.result()
        if winner == Disk.EMPTY:
            print_bold("The game ended in a tie...")
        else:
            print_bold(f"The winner is {str(winner)}!")

    def print_status(self):
        """Print current board and player info."""
        print(self.player_black)
        print(self.player_white, end="\n\n")
        print(self.board)

    @staticmethod
    def get_answer(question: str, yes="y", no="n") -> bool:
        """Ask a question with two options, and return bool from user answer."""
        ans = input(f"{question} ({yes}/{no})? ")
        return ans.lower() == yes.lower()

    @staticmethod
    def get_board_size() -> int:
        """Ask and return the desired board size."""
        while True:
            try:
                ans = int(input("Choose board board_size (default is 8): "))
                return clamp(ans, 4, 8)
            except ValueError:
                print_error("give a number...")


if __name__ == "__main__":
    print_bold("OTHELLO GAME - Python", Color.green)
    try:
        try:
            # try to read board size from command line args
            size = int(sys.argv[1])
            print(f"Using board size: {size}")
        except (ValueError, IndexError):
            size = Othello.get_board_size()

        game = Othello(size)
        game.play()
    except KeyboardInterrupt:
        sys.exit("\nGame stopped...")
