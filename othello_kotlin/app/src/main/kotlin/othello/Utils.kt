package othello

import java.awt.Color
import java.security.MessageDigest
import java.util.*

const val MIN_BOARD_SIZE = 4
const val MAX_BOARD_SIZE = 10
const val DEFAULT_BOARD_SIZE = 8

/** Represents one game piece or lack of one.*/
enum class Disk(val value: Int) {
    Black(-1),
    Empty(0),
    White(1),
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
    val directions: List<Step>,
) : Comparable<Move> {
    override fun compareTo(other: Move): Int {
        return when {
            value > other.value || (value == other.value && square < other.square) -> -1
            value < other.value || (value == other.value && square > other.square) -> 1
            else -> 0
        }
    }

    fun toLogEntry(): String {
        return "$disk:$square,$value"
    }

    override fun toString(): String {
        return "Square: $square -> value: $value"
    }
}

/** Game settings.*/
data class Settings(
    val boardSize: Int,
    val autoplayMode: Boolean,
    val quickStart: Boolean,
    val showHelpers: Boolean,
    val showLog: Boolean,
    val testMode: Boolean
)

/** Player settings.*/
data class PlayerSettings(
    val showHelpers: Boolean,
    val testMode: Boolean
)

fun Settings.toPlayerSettings(): PlayerSettings {
    return PlayerSettings(
        showHelpers = this.showHelpers,
        testMode = this.testMode
    )
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
fun Disk.boardChar(color: Boolean = true): String {
    return when (this) {
        Disk.Empty -> "_"
        Disk.White -> if (color) getColor("W", this.diskColor()) else "W"
        Disk.Black -> if (color) getColor("B", this.diskColor()) else "B"
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

fun calculateSha256(text: String): String {
    val messageDigest = MessageDigest.getInstance("SHA-256")
    val bytes = messageDigest.digest(text.toByteArray())

    val hexStringBuilder = StringBuilder()
    for (byte in bytes) {
        val hex = String.format("%02x", byte)
        hexStringBuilder.append(hex)
    }

    return hexStringBuilder.toString()
}

object BuildInfo {
    private val properties: Properties = Properties().apply {
        val propFileName = "build-info.properties"
        val classLoader = javaClass.classLoader ?: ClassLoader.getSystemClassLoader()
        classLoader.getResourceAsStream(propFileName)?.use {
            load(it)
        } ?: Properties()
    }

    val branch: String = properties.getProperty("build.branch", "unknown")
    val commit: String = properties.getProperty("build.commit", "unknown")
    val date: String = properties.getProperty("build.date", "unknown")
    val version: String = properties.getProperty("build.version", "unknown")
}

fun versionInfo(): String {
    return "${BuildInfo.version} ${BuildInfo.date} ${BuildInfo.branch} ${BuildInfo.commit}"
}
