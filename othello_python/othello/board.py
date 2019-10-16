"""
Class Board
Othello (Reversi) game implementation
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
"""
from util import Disk, Move
from colorprint import Color, get_color


class Board:
    def __init__(self, size):
        self._size = size
        self._board = [[Disk.EMPTY for _ in range(self._size)] for _ in range(self._size)]
        self.directions = ((0, -1), (0, 1), (1, 0), (-1, 0), (1, -1), (1, 1), (-1, 1), (-1, -1))

        # Set starting positions
        row = (self._size - 1) // 2
        col = self._size // 2
        self._board[row][row] = Disk.BLACK
        self._board[row][col] = Disk.WHITE
        self._board[col][row] = Disk.WHITE
        self._board[col][col] = Disk.BLACK

    def can_place_to_square(self, x: int, y: int, disk: Disk) -> bool:
        """Check can the given disk color be placed in the given position"""
        if self.get_square(x, y) == Disk.EMPTY:
            other = disk.other_disk()
            for i, j in self.directions:
                tx = x + i
                ty = y + j
                if self.get_square(tx, ty) == other:
                    while self.get_square(tx, ty) == other:
                        tx += i
                        ty += j
                    if self.get_square(tx, ty) == disk:
                        return True

        return False

    def check_coordinates(self, x: int, y: int) -> bool:
        """Check that the given coordinates are inside the board"""
        return 0 <= x < self._size and 0 <= y < self._size

    def get_possible_moves(self, disk: Disk):
        moves = []
        for y in range(self._size):
            for x in range(self._size):
                if self.get_square(x, y) == Disk.EMPTY:
                    other = disk.other_disk()
                    value = 0
                    for i, j in self.directions:
                        tx = x + i
                        ty = y + j
                        steps = 0
                        while self.get_square(tx, ty) == other:
                            tx += i
                            ty += j
                            steps += 1
                        if self.get_square(tx, ty) == disk:
                            value += steps

                    if value:
                        moves.append(Move(x, y, value))

        return moves

    def get_result(self) -> Disk:
        """Calculates the final score and returns the winner color"""
        total_score = self.get_score()
        if total_score > 0:
            return Disk.WHITE
        elif total_score < 0:
            return Disk.BLACK

        return Disk.EMPTY

    def get_score(self) -> int:
        """Returns the total score (positive means more white disks and negative means more black disks)"""
        return sum(sum(row) for row in self._board)

    def get_square(self, x: int, y: int):
        """Returns the state of the board (empty, white, black) at the given coordinates"""
        if self.check_coordinates(x, y):
            return self._board[y][x]

        return False

    def place_disk(self, x: int, y: int, disk) -> bool:
        """Tries to place the given disk color to the given square"""
        if self.can_place_to_square(x, y, disk):
            other = disk.other_disk()
            for i, j in self.directions:
                tx = x + i
                ty = y + j
                while self.get_square(tx, ty) == other:
                    tx += i
                    ty += j
                if self.get_square(tx, ty) == disk:
                    while (tx != x) or (ty != y):
                        tx -= i
                        ty -= j
                        self.set_square(tx, ty, disk)

            return True

        return False

    def set_square(self, x: int, y: int, disk: Disk):
        """Sets the given square to given value"""
        if self.check_coordinates(x, y):
            self._board[y][x] = disk
            return True

        return False

    def print_score(self):
        """ Count and print the number of black and white disks"""
        white = 0
        black = 0
        for row in self._board:
            for disk in row:
                if disk == Disk.WHITE:
                    white += 1
                elif disk == Disk.BLACK:
                    black += 1

        print("Score: {} | {}\n".format(get_color(str(white), Color.magenta), get_color(str(black), Color.cyan)))

    def __str__(self):
        """Format board state as a string"""
        board = f"  {' '.join(get_color(str(x), bold=True) for x in range(self._size))}\n"
        for i in range(self._size):
            board += get_color(f"{i} ", bold=True)
            board += ' '.join(repr(disk) for disk in self._board[i])
            board += "\n"

        return board
