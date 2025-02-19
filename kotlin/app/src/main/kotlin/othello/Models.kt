package othello

/** Represents one game piece or lack of one. */
enum class Disk(val value: Int) {
    Black(-1),
    Empty(0),
    White(1),
}

/** Represents a step direction on the board. */
data class Step(val x: Int, val y: Int) {
    override fun hashCode(): Int {
        var result = 17
        result = 31 * result + x.hashCode()
        result = 31 * result + y.hashCode()
        return result
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is Step) return false

        if (x != other.x) return false
        if (y != other.y) return false

        return true
    }

    override fun toString(): String = "[$x,$y]"
}

/** Represents one square location on the board. */
data class Square(val x: Int, val y: Int) : Comparable<Square> {
    override fun hashCode(): Int {
        var result = 17
        result = 31 * result + x.hashCode()
        result = 31 * result + y.hashCode()
        return result
    }

    operator fun plus(step: Step): Square = Square(x + step.x, y + step.y)

    operator fun plus(square: Square): Square = Square(x + square.x, y + square.y)

    override fun compareTo(other: Square): Int = when {
        x < other.x || (x <= other.x && y < other.y) -> -1
        x > other.x || (x >= other.x && y > other.y) -> 1
        else -> 0
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is Square) return false

        if (x != other.x) return false
        if (y != other.y) return false

        return true
    }

    override fun toString(): String = "($x,$y)"
}

/** Represents one possible disk placement for given disk color. */
data class Move(val square: Square, val value: Int, val disk: Disk, val directions: List<Step>) :
    Comparable<Move> {
    override fun compareTo(other: Move): Int = when {
        value > other.value || (value == other.value && square < other.square) -> -1
        value < other.value || (value == other.value && square > other.square) -> 1
        else -> 0
    }

    fun logEntry(): String = "${disk.boardChar(color = false)}:$square,$value"

    override fun toString(): String = "Square: $square -> value: $value"
}

/** Returns the print colour for the given Disk. */
fun Disk.diskColor(): AnsiColor = when (this) {
    Disk.Empty -> AnsiColor.WHITE
    Disk.White -> AnsiColor.CYAN
    Disk.Black -> AnsiColor.MAGENTA
}

/** Returns string character representing board status (black, white, empty). */
fun Disk.boardChar(color: Boolean = true): String = when (this) {
    Disk.Empty -> "_"
    Disk.White -> if (color) getColor("W", this.diskColor()) else "W"
    Disk.Black -> if (color) getColor("B", this.diskColor()) else "B"
}

/** Returns the disk formatted as a coloured string. */
fun Disk.name(): String = getColor(this.name.uppercase(), this.diskColor())

/** Returns the opposing disk colour. */
fun Disk.opponent(): Disk = when (this) {
    Disk.Empty -> Disk.Empty
    Disk.White -> Disk.Black
    Disk.Black -> Disk.White
}
