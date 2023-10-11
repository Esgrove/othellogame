package othello_kotlin

import java.awt.Color
import java.util.Objects

const val MIN_BOARD_SIZE = 4
const val MAX_BOARD_SIZE = 10
const val DEFAULT_BOARD_SIZE = 8

/** Represents one game piece or lack of one.*/
enum class Disk(val value: Int) {
    Black(-1),
    Empty(0),
    White(1)
}

/** Represents a step direction on the board.*/
data class Step(val x: Int, val y: Int) {
    override fun hashCode(): Int {
        return Objects.hash(x, y)
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Step

        if (x != other.x) return false
        if (y != other.y) return false

        return true
    }

    override fun toString(): String {
        return "[$x,$y]"
    }
}

/** Represents one square location on the board.*/
data class Square(val x: Int, val y: Int) : Comparable<Square> {
    override fun hashCode(): Int {
        return Objects.hash(x, y)
    }

    operator fun plus(step: Step): Square {
        return Square(x + step.x, y + step.y)
    }

    operator fun plus(square: Square): Square {
        return Square(x + square.x, y + square.y)
    }

    override fun compareTo(other: Square): Int {
        return when {
            x < other.x || (x <= other.x && y < other.y) -> -1
            x > other.x || (x >= other.x && y > other.y) -> 1
            else -> 0
        }
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Square

        if (x != other.x) return false
        if (y != other.y) return false

        return true
    }

    override fun toString(): String {
        return "($x,$y)"
    }
}

/** Represents one possible disk placement for given disk color.*/
data class Move(
    val square: Square,
    val value: Int,
    val disk: Disk,
    val directions: List<Step>
) : Comparable<Move> {
    override fun compareTo(other: Move): Int {
        return when {
            value > other.value || (value == other.value && square < other.square) -> -1
            value < other.value || (value == other.value && square > other.square) -> 1
            else -> 0
        }
    }

    override fun toString(): String {
        return "Square: $square -> value: $value"
    }
}

/** Returns the print colour for the given Disk.*/
fun Disk.diskColor(): Color {
    return when (this) {
        Disk.Empty -> Color.WHITE
        Disk.White -> Color.CYAN
        Disk.Black -> Color.MAGENTA
    }
}

/** Returns string character representing board status (black, white, empty).*/
fun Disk.boardChar(): String {
    return when (this) {
        Disk.Empty -> "_"
        Disk.White -> getColor("W", this.diskColor())
        Disk.Black -> getColor("B", this.diskColor())
    }
}

/** Returns the disk formatted as a coloured string.*/
fun Disk.name(): String {
    return getColor(this.name.uppercase(), this.diskColor())
}

/** Returns the opposing disk colour.*/
fun Disk.opponent(): Disk {
    return when (this) {
        Disk.Empty -> Disk.Empty
        Disk.White -> Disk.Black
        Disk.Black -> Disk.White
    }
}
