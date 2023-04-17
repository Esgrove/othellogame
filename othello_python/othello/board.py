"""
Class Board
Othello (Reversi) game implementation
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
2019-2023
"""

from colorprint import Color, get_color, print_color
from utils import Disk, Move, Square, Step


class Board:
    """Handles game board state and logic."""

    # Store all possible step directions on board
    STEP_DIRECTIONS = (
        Step(-1, -1),
        Step(-1, 0),
        Step(-1, 1),
        Step(0, -1),
        Step(0, 1),
        Step(1, -1),
        Step(1, 0),
        Step(1, 1),
    )

    def __init__(self, size=8):
        self._size = size
        # init game board with empty disks
        self._board = [[Disk.EMPTY for _ in range(self._size)] for _ in range(self._size)]

        # set starting positions
        row = (self._size - 1) // 2 if self._size % 2 == 0 else (self._size - 1) // 2 - 1
        col = self._size // 2
        self._board[row][row] = Disk.BLACK
        self._board[row][col] = Disk.WHITE
        self._board[col][row] = Disk.WHITE
        self._board[col][col] = Disk.BLACK

        # keep track of empty squares on board to avoid checking already filled positions
        self._empty_squares = {
            Square(x, y)
            for x in range(self._size)
            for y in range(self._size)
            if self._board[y][x] == Disk.EMPTY
        }

    def can_play(self) -> bool:
        """
        Return true if board contains empty squares.
        -> still possible to make a move.
        """
        return bool(self._empty_squares)

    def place_disk(self, player_move: Move) -> None:
        """Update board for given disk placement."""
        start = player_move.square
        assert (
            self._get_square(start) == Disk.EMPTY
        ), f"Trying to place disk to an occupied square {player_move.square}!"
        self._set_square(start, player_move.disk)
        self._empty_squares.remove(start)
        for step in player_move.directions:
            pos = start + step
            while self._get_square(pos) == player_move.disk.other_disk():
                self._set_square(pos, player_move.disk)
                pos += step

    def possible_moves(self, disk: Disk) -> list[Move]:
        """Returns a list of all possible moves for the given disk color."""
        moves = []
        for square in self._empty_squares:
            value = 0
            directions = []
            # try stepping in all directions from this starting point
            for step in self.STEP_DIRECTIONS:
                pos = square + step
                # not valid if next disk in line is own
                if self._get_square(pos) == disk:
                    continue
                steps = 0
                # keep stepping over opponents disks
                while self._get_square(pos) == disk.other_disk():
                    steps += 1
                    pos += step
                # successful move if this direction ends in own disk
                if self._get_square(pos) == disk:
                    value += steps
                    directions.append(step)

            if value:
                moves.append(Move(square, disk, value, directions))

        return sorted(moves)

    def print_moves(self, moves: list[Move]) -> None:
        """Print available move coordinates and resulting points gained."""
        print_color(f"  Possible moves ({len(moves)}):", Color.yellow)
        # convert board from Disk enums to strings
        board = [[disk.board_char() for disk in row] for row in self._board]
        for move in moves:
            print(f"  {move}")
            x, y = move.square
            # add move value to matching position on board
            board[y][x] = get_color(move.value, Color.yellow)

        # print board with move positions
        print(f"    {' '.join(get_color(str(x), bold=True) for x in range(self._size))}")
        for index, row in enumerate(board):
            text = get_color(f"  {index} ", bold=True)
            text += " ".join(disk for disk in row)
            print(text)

    def print_score(self) -> None:
        """Count and print the number of black and white disks."""
        black, white = self._player_scores()
        print(f"\n{self}")
        print(
            f"Score: {get_color(black, Disk.BLACK.color())} | "
            f"{get_color(white, Disk.WHITE.color())}"
        )

    def result(self) -> Disk:
        """Calculates the final score and returns the winning player disk."""
        total_score = self._score()
        if total_score == 0:
            return Disk.EMPTY

        return Disk.WHITE if total_score > 0 else Disk.BLACK

    def _check_coordinates(self, x: int, y: int) -> bool:
        """Check that the given coordinates are inside the board."""
        return 0 <= x < self._size and 0 <= y < self._size

    def _player_scores(self) -> tuple[int, int]:
        """Count and return the number of black and white disks."""
        white = 0  # sum(1 for d in self._board if d == Disk.WHITE)
        black = 0  # sum(1 for d in self._board if d == Disk.BLACK)
        for row in self._board:
            for disk in row:
                if disk == Disk.WHITE:
                    white += 1
                elif disk == Disk.BLACK:
                    black += 1

        return black, white

    def _score(self) -> int:
        """
        Returns the total score.
        Positive value means more white disks and negative means more black disks.
        """
        return sum(sum(row) for row in self._board)

    def _get_square(self, square: Square) -> Disk | None:
        """Returns the state of the board (empty, white, black) at the given square."""
        x, y = square
        return self._board[y][x] if self._check_coordinates(x, y) else None

    def _set_square(self, square: Square, disk: Disk) -> None:
        """Sets the given square to given value."""
        x, y = square
        if not self._check_coordinates(x, y):
            raise ValueError(f"Invalid coordinates: {square}!")
        self._board[y][x] = disk

    def __str__(self) -> str:
        """Format game board to string."""
        text = f"  {' '.join(get_color(str(x), bold=True) for x in range(self._size))}"
        for index, row in enumerate(self._board):
            text += get_color(f"\n{index} ", bold=True)
            text += " ".join(disk.board_char() for disk in row)
        return text
