package othello

/** Handles game board state and logic.*/
class Board(private val size: Int) {

    private val board: MutableList<Disk>
    private var emptySquares: HashSet<Square>
    private val indices: List<Int>

    companion object {
        // Store all possible step directions on board
        val stepDirections = arrayOf(
            Step(-1, -1),
            Step(-1, 0),
            Step(-1, 1),
            Step(0, -1),
            Step(0, 1),
            Step(1, -1),
            Step(1, 0),
            Step(1, 1),
        )
    }

    init {
        // Initialize game board with empty disks
        val numSquares = size * size
        board = MutableList(numSquares) { Disk.Empty }

        // Set starting positions
        val row = if (size % 2 == 0) (size - 1) / 2 else (size - 1) / 2 - 1
        val col = size / 2
        board[row * size + row] = Disk.White
        board[row * size + col] = Disk.Black
        board[col * size + row] = Disk.Black
        board[col * size + col] = Disk.White

        // Index list (0...size) to avoid repeating same range in loops
        indices = (0 until size).toList()

        // Keep track of empty squares on board to avoid checking already filled positions
        emptySquares = HashSet()
        for (y in indices) {
            for (x in indices) {
                if (board[y * size + x] == Disk.Empty) {
                    emptySquares.add(Square(x, y))
                }
            }
        }
    }

    /** Return true if board contains empty squares.*/
    fun canPlay(): Boolean {
        return emptySquares.isNotEmpty()
    }

    /** Update board for given disk placement.*/
    fun placeDisk(move: Move) {
        val start = move.square
        if (getSquare(start) != Disk.Empty) {
            throw Error("Trying to place disk to an occupied square $start!")
        }
        setSquare(start, move.disk)
        emptySquares.remove(start)
        for (dir in move.directions) {
            var pos = start + dir
            while (getSquare(pos) == move.disk.opponent()) {
                setSquare(pos, move.disk)
                pos += dir
            }
        }
    }

    /** Returns a list of possible moves for the given player.*/
    fun possibleMoves(color: Disk): List<Move> {
        val moves = mutableListOf<Move>()
        val other = color.opponent()
        for (square in emptySquares) {
            var value = 0
            val directions = mutableListOf<Step>()
            for (step in stepDirections) {
                var pos = square + step
                // Next square in this direction needs to be the opposing disk
                if (getSquare(pos) != other) {
                    continue
                }
                var numSteps = 0
                // Keep stepping over opponents disks
                while (getSquare(pos) == other) {
                    numSteps++
                    pos += step
                }
                // Valid move only if a line of opposing disks ends in own disk
                if (getSquare(pos) != color) {
                    continue
                }
                value += numSteps
                directions.add(step)
            }
            if (value > 0) {
                moves.add(Move(square, value, color, directions))
            }
        }
        return moves.sortedWith(
            compareByDescending<Move> { it.value }.thenBy { it.square.x }
                .thenBy { it.square.y },
        )
    }

    /** Print board with available move coordinates and the resulting points gained.*/
    fun printPossibleMoves(moves: Collection<Move>) {
        printColor("  Possible moves (${moves.size}):", AnsiColor.YELLOW)
        // Convert board from Disk enums to strings
        val formattedBoard = ArrayList<String>(board.size)
        formattedBoard.addAll(board.map { it.boardChar() })
        // Add possible moves to board
        for (move in moves) {
            println("  $move")
            val (x, y) = move.square
            formattedBoard[y * size + x] = getColor(move.value.toString(), AnsiColor.YELLOW)
        }
        // print board with move positions
        print("   ")
        for (column in indices) {
            print(" $column")
        }
        for (row in indices) {
            print("\n  $row")
            for (col in indices) {
                print(" ${formattedBoard[row * size + col]}")
            }
        }
        println("")
    }

    /** Print current score for both players.*/
    fun printScore() {
        val (black, white) = playerScores()
        println("\n$this")
        println(
            "Score: ${getColor(black.toString(), Disk.Black.diskColor())} | " +
                getColor(white.toString(), Disk.White.diskColor()),
        )
    }

    /** Calculates the final score and returns the winning player.*/
    fun result(): Disk {
        val sum = score()
        return when {
            sum < 0 -> Disk.Black
            sum > 0 -> Disk.White
            else -> Disk.Empty
        }
    }

    fun logEntry(): String {
        return board.joinToString("") { it.boardChar(color = false) }
    }

    /** Check that the given coordinates are inside the board.*/
    private fun checkCoordinates(x: Int, y: Int): Boolean {
        return x in 0..<size && y in 0..<size
    }

    /** Returns the state of the board (empty, white, black) at the given coordinates.*/
    private fun getSquare(square: Square): Disk? {
        val (x, y) = square
        return if (checkCoordinates(x, y)) {
            board[y * size + x]
        } else {
            null
        }
    }

    /** Count and return the number of black and white disks. */
    private fun playerScores(): Pair<Int, Int> {
        var black = 0
        var white = 0
        for (disk in board) {
            when (disk) {
                Disk.White -> ++white
                Disk.Black -> ++black
                Disk.Empty -> {}
            }
        }
        return Pair(black, white)
    }

    /**
     * Returns the total score.
     * Positive value means more white disks and negative means more black disks.
     */
    private fun score(): Int {
        return board.sumOf { it.value }
    }

    /** Sets the given square to given value.*/
    private fun setSquare(square: Square, disk: Disk) {
        val (x, y) = square
        if (!checkCoordinates(x, y)) {
            throw Error("Invalid coordinates ($x,$y)!")
        }
        board[y * size + x] = disk
    }

    override fun toString(): String {
        val builder = StringBuilder(" ")
        for (column in indices) {
            builder.append(" $column")
        }
        for (row in indices) {
            builder.append("\n$row")
            for (col in indices) {
                builder.append(" ${board[row * size + col].boardChar()}")
            }
        }
        return builder.toString()
    }
}
