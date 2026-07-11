package othello

/** Represents one game piece or lack of one. */
enum class Disk(val value: Int) {
    Black(-1),
    Empty(0),
    White(1),
    ;

    /** Returns a single character identifier string for the given disk. */
    fun boardChar(): String = when (this) {
        Black -> "B"
        Empty -> "_"
        White -> "W"
    }

    /** Returns a coloured single character identifier string for the given disk. */
    fun boardCharWithColor(): String = getColor(boardChar(), diskColor())

    /** Return the associated colour for this disk. */
    fun diskColor(): AnsiColor = when (this) {
        Black -> AnsiColor.MAGENTA
        Empty -> AnsiColor.WHITE
        White -> AnsiColor.CYAN
    }

    /** Returns the disk formatted as a coloured string. */
    fun diskString(): String = getColor(name.uppercase(), diskColor())

    /** Return the opposing disk colour for this disk. */
    fun opponent(): Disk = when (this) {
        Black -> White
        Empty -> Empty
        White -> Black
    }
}

/** Represents one step direction on the board. */
data class Step(val x: Int, val y: Int) : Comparable<Step> {
    operator fun plus(other: Step): Step = Step(x + other.x, y + other.y)

    override fun compareTo(other: Step): Int = compareValuesBy(this, other, { it.x }, { it.y })

    override fun toString(): String = "[$x,$y]"
}

/** Represents one square location on the board. */
data class Square(val x: Int, val y: Int) : Comparable<Square> {
    /** Get the index of this square on the board. */
    fun boardIndex(boardSize: Int): Int = y * boardSize + x

    operator fun plus(other: Square): Square = Square(x + other.x, y + other.y)

    operator fun plus(step: Step): Square = Square(x + step.x, y + step.y)

    override fun compareTo(other: Square): Int = compareValuesBy(this, other, { it.x }, { it.y })

    override fun toString(): String = "($x,$y)"
}

/**
 * Represents a continuous line of squares in one direction.
 *
 * The [step] field determines the direction on the board,
 * and [count] describes how many consecutive squares in that direction there are.
 */
data class Direction(
    /** Direction of travel on the board */
    val step: Step,
    /** Number of consecutive same colour squares along this direction */
    val count: Int,
) : Comparable<Direction> {
    override fun compareTo(other: Direction): Int = when {
        step != other.step -> step.compareTo(other.step)
        else -> count.compareTo(other.count)
    }
}

/** Represents one possible disk placement for the given disk colour. */
class Move(val square: Square, val disk: Disk, val value: Int, val directions: List<Direction>) :
    Comparable<Move> {
    /** Format move for log entry */
    fun logEntry(): String = "${disk.boardChar()}:$square,$value"

    /** Get all the squares playing this move will change. */
    fun affectedSquares(): List<Square> {
        val paths = mutableListOf<Square>()
        for ((step, count) in directions) {
            var pos = square + step
            repeat(count) {
                paths.add(pos)
                pos += step
            }
        }
        return paths.sorted()
    }

    override fun compareTo(other: Move): Int = when {
        value > other.value || (value == other.value && square < other.square) -> -1
        value < other.value || (value == other.value && square > other.square) -> 1
        else -> 0
    }

    override fun equals(other: Any?): Boolean = other is Move &&
        square == other.square &&
        value == other.value &&
        disk == other.disk

    override fun hashCode(): Int {
        var result = square.hashCode()
        result = 31 * result + value.hashCode()
        result = 31 * result + disk.hashCode()
        return result
    }

    override fun toString(): String = "Square: $square -> value: $value"
}
