package othello

/** Handles game board state and logic. */
class Board(internal val size: Int) {
    private val board: MutableList<Disk>
    private var emptySquares: HashSet<Square>
    private val indices: List<Int>

    companion object {
        private const val UP: Int = 1
        private const val DOWN: Int = -1
        private const val LEFT: Int = -1
        private const val RIGHT: Int = 1
        private const val STILL: Int = 0

        /** All possible step directions for a square on the board. */
        val STEP_DIRECTIONS = arrayOf(
            Step(DOWN, LEFT),
            Step(DOWN, RIGHT),
            Step(DOWN, STILL),
            Step(STILL, LEFT),
            Step(STILL, RIGHT),
            Step(UP, LEFT),
            Step(UP, RIGHT),
            Step(UP, STILL),
        )
    }

    /** Initialize a new board for the given board size. */
    init {
        board = initBoard()

        // Index list (0...size) to avoid repeating same range in loops.
        indices = (0 until size).toList()

        // Keep track of empty squares on board to avoid checking already filled positions.
        emptySquares = initEmptySquares()
    }

    /** Return true if board contains empty squares. */
    fun canPlay(): Boolean = emptySquares.isNotEmpty()

    /** Update board for given disk placement. */
    fun placeDisk(chosenMove: Move) {
        val start = chosenMove.square
        val square = getSquare(start) ?: throw Error("Invalid coordinates: $start")
        if (square != Disk.Empty) {
            throw Error("Trying to place disk to an occupied square: $start!")
        }
        setSquare(start, chosenMove.disk)
        emptySquares.remove(start)
        for (affected in chosenMove.affectedSquares()) {
            setSquare(affected, chosenMove.disk)
        }
    }

    /** Returns a list of possible moves for the given player. */
    fun possibleMoves(disk: Disk): List<Move> {
        val moves = mutableListOf<Move>()
        val opposingDisk = disk.opponent()
        for (square in emptySquares) {
            var value = 0
            val directions = mutableListOf<Direction>()
            for (step in STEP_DIRECTIONS) {
                var pos = square + step
                // Next square in this direction needs to be the opposing disk
                if (getSquare(pos) != opposingDisk) {
                    continue
                }
                var numSteps = 0
                // Keep stepping over opponents disks
                while (getSquare(pos) == opposingDisk) {
                    numSteps++
                    pos += step
                }
                // Valid move only if a line of opposing disks ends with own disk
                if (getSquare(pos) == disk) {
                    directions.add(Direction(step, numSteps))
                    value += numSteps
                }
            }
            if (value > 0) {
                moves.add(Move(square, disk, value, directions))
            }
        }
        return moves.sorted()
    }

    /** Print board with available move coordinates and the resulting points gained. */
    fun printPossibleMoves(moves: Collection<Move>) {
        printYellow("  Possible moves (${moves.size}):")
        // Convert board from Disk enums to strings
        val formattedBoard = ArrayList<String>(board.size)
        formattedBoard.addAll(board.map { it.boardCharWithColor() })

        // Add possible moves to board
        for (possibleMove in moves) {
            val index = squareIndex(possibleMove.square)
            formattedBoard[index] = getColor(possibleMove.value.toString(), AnsiColor.YELLOW)
            println("  $possibleMove")
        }

        // Print board with move positions
        val boldIndices = indices.map { getColor(it.toString(), AnsiColor.BOLD) }
        val header = (listOf("    ") + boldIndices).joinToString(" ")
        val rows = indices.joinToString("\n") { y ->
            val row = indices.joinToString(" ") { x -> formattedBoard[y * size + x] }
            "  ${getColor(y.toString(), AnsiColor.BOLD)} $row"
        }
        println("$header\n$rows")
    }

    /** Print current score for both players. */
    fun printScore() {
        val (black, white) = playerScores()
        println("\n$this")
        println(
            "Score: ${getColor(black.toString(), Disk.Black.diskColor())} | " +
                getColor(white.toString(), Disk.White.diskColor()),
        )
    }

    /** Returns the winning disk colour. Empty indicates a draw. */
    fun result(): Disk {
        val totalScore = score()
        return when {
            totalScore > 0 -> Disk.White
            totalScore < 0 -> Disk.Black
            else -> Disk.Empty
        }
    }

    /** Get board status string for game log. */
    fun logEntry(): String = board.joinToString("") { it.boardChar() }

    /** Check that the given coordinates are valid (inside the board). */
    private fun checkCoordinates(x: Int, y: Int): Boolean = x in 0..<size && y in 0..<size

    /** Check that the given square is valid (inside the board). */
    private fun checkSquare(square: Square): Boolean = checkCoordinates(square.x, square.y)

    /** Returns the state of the board (empty, white, black) at the given square. */
    internal fun getSquare(square: Square): Disk? = if (checkSquare(square)) {
        board[squareIndex(square)]
    } else {
        null
    }

    /** Map square to board index. */
    private fun squareIndex(square: Square): Int = square.y * size + square.x

    /** Count and return the number of black and white disks. */
    internal fun playerScores(): Pair<Int, Int> {
        var black = 0
        var white = 0
        for (disk in board) {
            when (disk) {
                Disk.Black -> black++
                Disk.White -> white++
                Disk.Empty -> {}
            }
        }
        return Pair(black, white)
    }

    /**
     * Returns the total score.
     * Positive value means more white disks and negative means more black disks.
     */
    internal fun score(): Int = board.sumOf { it.value }

    /** Sets the given square to the given value. */
    private fun setSquare(square: Square, disk: Disk) {
        if (!checkSquare(square)) {
            throw Error("Invalid coordinates: $square")
        }
        board[squareIndex(square)] = disk
    }

    /** Initialize game board with starting disk positions. */
    private fun initBoard(): MutableList<Disk> {
        // Initialize game board with empty disks
        val board = MutableList(size * size) { Disk.Empty }

        // Set starting positions
        val row = if (size % 2 == 0) (size - 1) / 2 else (size - 1) / 2 - 1
        val col = size / 2
        board[row * size + row] = Disk.White
        board[row * size + col] = Disk.Black
        board[col * size + row] = Disk.Black
        board[col * size + col] = Disk.White

        return board
    }

    /** Initialize empty squares for the board. */
    private fun initEmptySquares(): HashSet<Square> {
        val emptySquares = HashSet<Square>()
        for (y in indices) {
            for (x in indices) {
                if (board[y * size + x] == Disk.Empty) {
                    emptySquares.add(Square(x, y))
                }
            }
        }
        return emptySquares
    }

    override fun toString(): String = buildString {
        // Horizontal indices
        append("  ${getColor(this@Board.indices.joinToString(" "), AnsiColor.BOLD)}")
        for (y in this@Board.indices) {
            // Vertical index
            append("\n${getColor(y.toString(), AnsiColor.BOLD)}")
            // Row values
            for (x in this@Board.indices) {
                append(" ${board[y * size + x].boardCharWithColor()}")
            }
        }
    }
}
