"""
Class Board
Defines the game board
Akseli Lukkarila
2019-2026
"""

try:
    from othello.colorprint import Color, get_color, print_yellow
    from othello.models import Direction, Disk, Move, Square, Step
except ModuleNotFoundError:
    from colorprint import Color, get_color, print_yellow
    from models import Direction, Disk, Move, Square, Step

UP = 1
DOWN = -1
LEFT = -1
RIGHT = 1
STILL = 0

# All possible step directions for a square on the board.
STEP_DIRECTIONS = (
    Step(DOWN, LEFT),
    Step(DOWN, RIGHT),
    Step(DOWN, STILL),
    Step(STILL, LEFT),
    Step(STILL, RIGHT),
    Step(UP, LEFT),
    Step(UP, RIGHT),
    Step(UP, STILL),
)


class Board:
    """Handles game board state and logic."""

    def __init__(self, size=8):
        """Initialize a new board for the given board size."""
        board = self._init_board(size)
        # Index list (0...size) to avoid repeating same range in loops.
        # Not really needed in Python but kept here to more closely match other implementations...
        indices = tuple(range(size))

        # Keep track of empty squares on board to avoid checking already filled positions.
        empty_squares = self._init_empty_squares(size, board)

        self._board: list[Disk] = board
        self._empty_squares: set[Square] = empty_squares
        self._indices: tuple[int, ...] = indices
        self._size: int = size

    def can_play(self) -> bool:
        """Return true if board contains empty squares."""
        return bool(self._empty_squares)

    def place_disk(self, chosen_move: Move) -> None:
        """
        Update board for given disk placement.
        Throws AssertionError if the move is not a valid move.
        """
        start = chosen_move.square
        assert self._check_square(start), f"Invalid coordinates: {start}"
        assert self._get_square(start) == Disk.EMPTY, (
            f"Trying to place disk to an occupied square: {start}!"
        )
        self._set_square(start, chosen_move.disk)
        self._empty_squares.remove(start)
        for square in chosen_move.affected_squares():
            self._set_square(square, chosen_move.disk)

    def possible_moves(self, disk: Disk) -> list[Move]:
        """Returns a list of possible moves for the given player."""
        moves = []
        opposing_disk = disk.opponent()
        for square in self._empty_squares:
            value = 0
            directions = []
            for step in STEP_DIRECTIONS:
                pos = square + step
                # Next square in this direction needs to be the opposing disk
                if self._get_square(pos) != opposing_disk:
                    continue

                num_steps = 0
                # Keep stepping over opponents disks
                while self._get_square(pos) == opposing_disk:
                    num_steps += 1
                    pos += step
                # Valid move only if a line of opposing disks ends with own disk
                if self._get_square(pos) == disk:
                    directions.append(Direction(step, num_steps))
                    value += num_steps

            if value:
                moves.append(Move(square, disk, value, directions))

        return sorted(moves)

    def print_possible_moves(self, moves: list[Move]) -> None:
        """Print board with available move coordinates and the resulting points gained."""
        print_yellow(f"  Possible moves ({len(moves)}):")
        # Convert board from Disk enums to strings
        formatted_board = [disk.board_char_with_color() for disk in self._board]
        # Add possible moves to board
        for possible_move in moves:
            index = self._square_index(possible_move.square)
            formatted_board[index] = get_color(str(possible_move.value), Color.yellow)
            print(f"  {possible_move}")

        # Print board with move positions
        header = " ".join(["    "] + [get_color(str(i), bold=True) for i in self._indices])
        board = "\n".join(
            f"  {get_color(str(y), bold=True)} "
            + " ".join(formatted_board[y * self._size + x] for x in self._indices)
            for y in self._indices
        )
        print(f"{header}\n{board}")

    def print_score(self) -> None:
        """Print current score for both players."""
        black, white = self._player_scores()
        print(f"\n{self}")
        print(
            f"Score: {get_color(str(black), Color.magenta)} | {get_color(str(white), Color.cyan)}"
        )

    def result(self) -> Disk:
        """Returns the winning disk colour. Empty indicates a draw."""
        match self._score():
            case 0:
                return Disk.EMPTY
            case score if score > 0:
                return Disk.WHITE
            case _:
                return Disk.BLACK

    def log_entry(self) -> str:
        """Get board status string for game log."""
        return "".join(disk.board_char() for disk in self._board)

    def _check_coordinates(self, x: int, y: int) -> bool:
        """Check that the given coordinates are valid (inside the board)."""
        return 0 <= x < self._size and 0 <= y < self._size

    def _check_square(self, square: Square) -> bool:
        """Check that the given square is valid (inside the board)."""
        return self._check_coordinates(square.x, square.y)

    def _get_square(self, square: Square) -> Disk | None:
        """Returns the state of the board (empty, white, black) at the given square."""
        if self._check_square(square):
            index = self._square_index(square)
            return self._board[index]

        return None

    def _square_index(self, square: Square) -> int:
        """Map square to board index."""
        return square.y * self._size + square.x

    def _player_scores(self) -> tuple[int, int]:
        """Count and return the number of black and white disks."""
        # Alternative that is more readable but iterates the board twice:
        # black = sum(disk == Disk.BLACK for disk in self._board)
        # white = sum(disk == Disk.WHITE for disk in self._board)
        black = 0
        white = 0
        for disk in self._board:
            if disk == Disk.BLACK:
                black += 1
            elif disk == Disk.WHITE:
                white += 1

        return black, white

    def _score(self) -> int:
        """
        Returns the total score.
        Positive value means more white disks and negative means more black disks.
        """
        return sum(disk.value for disk in self._board)

    def _set_square(self, square: Square, disk: Disk) -> None:
        """Sets the given square to the given value."""
        assert self._check_square(square), f"Invalid coordinates: {square}"
        index = self._square_index(square)
        self._board[index] = disk

    @staticmethod
    def _init_board(size: int) -> list[Disk]:
        """Initialize game board with starting disk positions."""
        # Initialize game board with empty disks
        board = [Disk.EMPTY for _ in range(size * size)]
        # Set starting positions
        row = (size - 1) // 2 if size % 2 == 0 else (size - 1) // 2 - 1
        col = size // 2
        board[row * size + row] = Disk.WHITE
        board[row * size + col] = Disk.BLACK
        board[col * size + row] = Disk.BLACK
        board[col * size + col] = Disk.WHITE
        return board

    @staticmethod
    def _init_empty_squares(size: int, board: list[Disk]) -> set[Square]:
        """Initialize empty squares for the board."""
        return {
            Square(x, y)
            for x in range(size)
            for y in range(size)
            if board[y * size + x] == Disk.EMPTY
        }

    def __str__(self) -> str:
        """Format game board to string."""
        # Horizontal indices
        board = f"  {get_color(' '.join(str(i) for i in self._indices), bold=True)}"
        for y in self._indices:
            # Vertical index
            board += f"\n{get_color(str(y), bold=True)}"
            # Row values
            for x in self._indices:
                board += f" {self._board[y * self._size + x].board_char_with_color()}"

        return board
