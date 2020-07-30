"""
Class Board
Othello (Reversi) game implementation
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019
"""
from typing import List, Optional

from colorprint import Color, get_color, print_color
from util import Disk, Move, Square


class Board:
    """Handles game board state and logic."""
    DIRECTIONS = ((0, -1), (0, 1), (1, 0), (-1, 0), (1, -1), (1, 1), (-1, 1), (-1, -1))

    def __init__(self, size=8):
        self._size = size
        self._board = [[Disk.EMPTY for _ in range(self._size)] for _ in range(self._size)]

        # set starting positions
        row = (self._size - 1) // 2 if self._size % 2 == 0 else (self._size - 1) // 2 - 1
        col = self._size // 2
        self._board[row][row] = Disk.BLACK
        self._board[row][col] = Disk.WHITE
        self._board[col][row] = Disk.WHITE
        self._board[col][col] = Disk.BLACK

        # keep a list of empty squares to avoid having to iterate through the whole board constantly
        # when checking for available moves
        self.empty_squares = set((x, y) for x in range(self._size) for y in range(self._size) if
                                 self._board[y][x] == Disk.EMPTY)

    def can_play(self) -> bool:
        """Return true if board contains empty squares -> still possible to make a move."""
        return bool(self.empty_squares)

    def check_coordinates(self, x: int, y: int) -> bool:
        """Check that the given coordinates are inside the board."""
        return 0 <= x < self._size and 0 <= y < self._size

    def place_disk(self, x: int, y: int, disk: Disk, directions: List):
        """Update board for given disk placement."""
        assert self.square(x, y) == Disk.EMPTY, "Trying to place a disk to an occupied square!"
        self.set_square(x, y, disk)
        self.empty_squares.remove((x, y))
        for i, j in directions:
            tx = x + i
            ty = y + j
            while self.square(tx, ty) == disk.other_disk():
                self.set_square(tx, ty, disk)
                tx += i
                ty += j

    def possible_moves(self, disk: Disk) -> List[Move]:
        """Returns a list of all possible moves for the given disk color."""
        moves = []
        for x, y in self.empty_squares:
            value = 0
            directions = []
            for i, j in self.DIRECTIONS:
                tx = x + i
                ty = y + j
                steps = 0
                while self.square(tx, ty) == disk.other_disk():
                    tx += i
                    ty += j
                    steps += 1
                if self.square(tx, ty) == disk:
                    value += steps
                    directions.append((i, j))

            if value:
                moves.append(Move(Square(x, y), disk, value, directions))

        return moves

    def print_possible_moves(self, moves: List[Move]):
        """Print available move coordinates and resulting points gained."""
        # convert board from Disk enums to strings
        board = [[disk.board_char() for disk in row] for row in self._board]

        # add value gained to matching position on board
        for move in moves:
            x, y = move.square
            board[y][x] = get_color(str(move.value), Color.yellow)

        print_color(f"  Possible plays ({len(moves)}):", Color.yellow)
        print(f"    {' '.join(get_color(str(x), bold=True) for x in range(self._size))}")
        for index, row in enumerate(board):
            text = get_color(f"  {index} ", bold=True)
            text += ' '.join(disk for disk in row)
            print(text)

        # print coordinate info
        for move in sorted(moves):
            print(f"  {move}")

    def print_score(self):
        """ Count and print the number of black and white disks."""
        white = 0
        black = 0
        for row in self._board:
            for disk in row:
                if disk == Disk.WHITE:
                    white += 1
                elif disk == Disk.BLACK:
                    black += 1

        print(f"Score: {get_color(str(black), Disk.BLACK.color())} | {get_color(str(white), Disk.WHITE.color())}")

    def result(self) -> Disk:
        """Calculates the final score and returns the winning player color."""
        total_score = self.score()
        if total_score == 0:
            return Disk.EMPTY

        return Disk.WHITE if total_score > 0 else Disk.BLACK

    def score(self) -> int:
        """Returns the total score (positive means more white disks and negative means more black disks)."""
        return sum(sum(row) for row in self._board)

    def set_square(self, x: int, y: int, disk: Disk):
        """Sets the given square to given value."""
        if not self.check_coordinates(x, y):
            raise ValueError(f"Invalid coordinates ({x},{y})!")
        self._board[y][x] = disk

    def square(self, x: int, y: int) -> Optional[Disk]:
        """Returns the state of the board (empty, white, black) at the given coordinates."""
        return self._board[y][x] if self.check_coordinates(x, y) else None

    def __str__(self):
        text = f"  {' '.join(get_color(str(x), bold=True) for x in range(self._size))}"
        for index, row in enumerate(self._board):
            text += get_color(f"\n{index} ", bold=True)
            text += ' '.join(disk.board_char() for disk in row)
        return text
