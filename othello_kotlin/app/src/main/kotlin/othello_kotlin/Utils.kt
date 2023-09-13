package othello_kotlin

import java.awt.Color
import java.util.Objects

// Represents one game piece or lack of one.
enum class Disk(val value: Int) {
    Black(-1),
    Empty(0),
    White(1)
}

// Represents one step direction on the board.
data class Step(val x: Int, val y: Int) {
    override fun hashCode(): Int {
        return Objects.hash(x, y)
    }
}

// Represents one square location on the board.
data class Square(val x: Int, val y: Int) : Comparable<Square> {
    override fun hashCode(): Int {
        return Objects.hash(x, y)
    }

    operator fun plus(other: Square): Square {
        return Square(x + other.x, y + other.y)
    }

    override fun compareTo(other: Square): Int {
        return when {
            x < other.x || (x <= other.x && y < other.y) -> -1
            x > other.x || (x >= other.x && y > other.y) -> 1
            else -> 0
        }
    }
}

// Represents one possible disk placement for given disk color.
data class Move(
    val square: Square,
    val value: Int,
    val disk: Disk,
    val directions: List<Square>
) : Comparable<Move> {
    override fun compareTo(other: Move): Int {
        return when {
            value > other.value || (value == other.value && square < other.square) -> -1
            value < other.value || (value == other.value && square > other.square) -> 1
            else -> 0
        }
    }
}

// Extensions for the Disk class
fun Disk.diskColor(): Color {
    return when (this) {
        Disk.Empty -> Color.WHITE
        Disk.White -> Color.CYAN
        Disk.Black -> Color.MAGENTA
    }
}

fun Disk.otherDisk(): Disk {
    return when (this) {
        Disk.Empty -> Disk.Empty
        Disk.White -> Disk.Black
        Disk.Black -> Disk.White
    }
}

fun Disk.name(): String {
    return colorPrint(this.name.uppercase(), this.diskColor())
}

fun Disk.boardChar(): String {
    return when (this) {
        Disk.Empty -> "_"
        Disk.White -> colorPrint("W", this.diskColor())
        Disk.Black -> colorPrint("B", this.diskColor())
    }
}
