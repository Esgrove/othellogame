"""
Class Othello
Othello (Reversi) game implementation
https://en.wikipedia.org/wiki/Reversi
Akseli Lukkarila
"""
from util import Disk, Move
from color import Color, getColor, getBold
#===========================================================

class Othello:
    def __init__(self, size):
        self._size = size
        self._board = [[Disk.EMPTY for x in range(self._size)] for y in range(self._size)]
        self.directions = ((0, -1), (0, 1), (1, 0), (-1, 0), (1, -1), (1, 1), (-1, 1), (-1, -1))
        self.initBoard()

    # ------------------------------------------------------

    def initBoard(self):
        """Set starting positions for board"""
        row = (self._size - 1) // 2
        col = self._size // 2
        self._board[row][row] = Disk.BLACK
        self._board[row][col] = Disk.WHITE
        self._board[col][row] = Disk.WHITE
        self._board[col][col] = Disk.BLACK

    # ------------------------------------------------------

    def canPlaceToSquare(self, rx, ry, disk):
        """Check can the given disk color be placed in the given position"""
        if self.getSquare(rx, ry) == Disk.EMPTY:
            other = self.getOtherDisk(disk)
            for i,j in self.directions:
                x = rx + i
                y = ry + j
                if self.getSquare(x, y) == other:
                    while self.getSquare(x, y) == other:
                        x += i
                        y += j
                    if self.getSquare(x, y) == disk:
                        return True

        return False

    # ------------------------------------------------------

    def canPlaceDisk(self, disk):
        """Checks if it is possible to place a given disk on the board"""
        for x in range(self._size):
            for y in range(self._size):
                if self.canPlaceToSquare(x, y, disk):
                    return True

        return False

    # ------------------------------------------------------

    def checkCoordinates(self, x, y):
        """Check that the given coordinates are inside the board"""
        return 0 <= x < self._size and 0 <= y < self._size

    # ------------------------------------------------------

    def getBoardString(self, disk):
        """Returns the identifier string for the given disk"""
        color = "_"
        if disk == Disk.BLACK:
            color = getColor("B", Color.cyan)
        elif disk == Disk.WHITE:
            color = getColor("W", Color.magenta)
        return color

    # ------------------------------------------------------

    def getDiskString(self, disk):
        """Returns the disk color as a string"""
        return "black" if disk == Disk.BLACK else "white"

    # ------------------------------------------------------

    def getOtherDisk(self, disk):
        """Returns the opponents color"""
        return Disk.WHITE if disk == Disk.BLACK else Disk.BLACK

    # ------------------------------------------------------

    def getPossibleMoves(self, disk):
        moves = []
        for ry in range(self._size):
            for rx in range(self._size):
                if self.getSquare(rx, ry) == Disk.EMPTY:
                    other = self.getOtherDisk(disk)
                    value = 0
                    for i,j in self.directions:
                        x = rx + i
                        y = ry + j
                        steps = 0
                        while self.getSquare(x, y) == other:
                            x += i
                            y += j
                            steps += 1
                        if self.getSquare(x, y) == disk:
                            value += steps

                    if value:
                        moves.append(Move(rx, ry, value))

        return moves

    # ------------------------------------------------------

    def getResult(self):
        """Calculates the final score and returns the winner color"""
        totalScore = self.getScore()
        if totalScore > 0:
            return Disk.WHITE
        elif totalScore < 0:
            return Disk.BLACK

        return Disk.EMPTY

    # ------------------------------------------------------

    def getScore(self):
        """Returns the total score (positive means more white disks and negative means more black disks)"""
        return sum(sum(row) for row in self._board)

    # ------------------------------------------------------

    def getSquare(self, x, y):
        """Returns the state of the board (empty, white, black) at the given coordinates"""
        if self.checkCoordinates(x, y):
            return self._board[y][x]

        return False

    # ------------------------------------------------------

    def placeDisk(self, rx, ry, disk):
        """Tries to place the given disk color to the given square"""
        if self.canPlaceToSquare(rx, ry, disk):
            other = self.getOtherDisk(disk)
            for i,j in self.directions:
                x = rx + i
                y = ry + j
                while self.getSquare(x, y) == other:
                    x += i
                    y += j
                if self.getSquare(x, y) == disk:
                    while (x != rx) or (y != ry):
                        x -= i
                        y -= j
                        self.setSquare(x, y, disk)

            return True

        return False

    # ------------------------------------------------------

    def setSquare(self, x, y, disk):
        """Sets the given square to given value"""
        if self.checkCoordinates(x, y):
            self._board[y][x] = disk
            return True

        return False

    # ------------------------------------------------------

    def __str__(self):
        """Format board state as a string"""
        board = "  " + " ".join(getBold(str(x)) for x in range(self._size)) + "\n"
        for i in range(self._size):
            board += getBold(str(i)) + " " + " ".join(self.getBoardString(x) for x in self._board[i]) + "\n"

        return board

    # ------------------------------------------------------

    def printBoard(self):
        print(self)

    # ------------------------------------------------------

    def printScore(self):
        white = 0
        black = 0
        for row in self._board:
            for disk in row:
                if disk == Disk.WHITE:
                    white += 1
                elif disk == Disk.BLACK:
                    black += 1

        print("Score: W = {}, B = {}\n".format(white, black))
