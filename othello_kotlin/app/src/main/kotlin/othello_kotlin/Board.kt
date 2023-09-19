package othello_kotlin

import java.awt.Color
import java.lang.IllegalArgumentException

/** */
class Board(private val size: Int) {

    private val board: MutableList<Disk>
    private lateinit var emptySquares: HashSet<Square>
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
        // Initialize game board with empty disks.
        val numSquares = size * size
        board = MutableList(numSquares) { Disk.Empty }

        // Set starting positions
        val row = if (size % 2 == 0) (size - 1) / 2 else (size - 1) / 2 - 1
        val col = size / 2
        board[row * size + row] = Disk.Black
        board[row * size + col] = Disk.White
        board[col * size + row] = Disk.White
        board[col * size + col] = Disk.Black

        // Index list (0...size) to avoid repeating same range in loops.
        indices = (0 until size).toList()

        // Keep track of empty squares on board to avoid checking already filled positions
        for (y in indices) {
            for (x in indices) {
                if (board[y * size + x] == Disk.Empty) {
                    emptySquares.add(Square(x, y))
                }
            }
        }
    }

    /** */
    fun canPlay(): Boolean {
        return emptySquares.isNotEmpty()
    }

    /** */
    fun placeDisc(move: Move) {
        val start = move.square
        if (getSquare(start) != Disk.Empty) {
            throw IllegalArgumentException("Trying to place disk to an occupied square $start!")
        }
        setSquare(start, move.disk)
        emptySquares.remove(start)
        for (dir in move.directions) {
            var pos = start + dir
            while (getSquare(pos) == move.disk.otherDisk()) {
                setSquare(pos, move.disk)
                pos += dir
            }
        }
    }

    /** */
    fun possibleMoves(color: Disk): List<Move> {
        val moves = mutableListOf<Move>()
        val other = color.otherDisk()
        for (square in emptySquares) {
            var value = 0
            val directions = mutableListOf<Square>()
            for (dir in stepDirections) {
                val step = Square(dir.x, dir.y)
                var pos = square + step
                if (getSquare(pos) != other) {
                    continue
                }
                var steps = 0
                while (getSquare(pos) == other) {
                    steps++
                    pos += step
                }
                if (getSquare(pos) != color) {
                    continue
                }
                value += steps
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

    /** */
    fun printMoves(moves: Collection<Move>) {
        printColor("  Possible moves (${moves.size}):", Color.YELLOW)

        // convert board from Disk enums to strings
        val boardStr = ArrayList<String>(board.size)
        boardStr.addAll(board.map { it.boardChar() })

        for (move in moves) {
            println("  $move")
            val (x, y) = move.square
            boardStr[y * size + x] = getColor(move.value.toString(), Color.YELLOW)
        }

        // print board with move positions
        print("   ")
        for (i in indices) {
            print(" $i")
        }

        for (y in indices) {
            print("\n  $y")
            for (x in indices) {
                print(" ${boardStr[y * size + x]}")
            }
        }

        println("")
    }

    /** */
    private fun checkCoordinates(x: Int, y: Int): Boolean {
        return x in 0 until size && y in 0 until size
    }

    /** */
    private fun getSquare(square: Square): Disk? {
        val (x, y) = square
        return if (!checkCoordinates(x, y)) {
            null
        } else {
            board[y * size + x]
        }
    }

    /** */
    private fun setSquare(square: Square, disk: Disk) {
        val (x, y) = square
        if (!checkCoordinates(x, y)) {
            throw IllegalArgumentException("Invalid coordinates ($x,$y)!")
        }
        board[y * size + x] = disk
    }

    /** */
    override fun toString(): String {
        val builder = StringBuilder(" ")
        for (i in indices) {
            builder.append(" $i")
        }
        for (y in indices) {
            builder.append("\n$y")
            for (x in indices) {
                builder.append(" ${board[y * size + x].boardChar()}")
            }
        }
        return builder.toString()
    }

    /** */
    fun printScore() {
        TODO("Not yet implemented")
    }
}
