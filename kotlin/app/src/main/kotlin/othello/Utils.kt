package othello

import okio.Buffer
import okio.HashingSink
import okio.buffer
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

/** Represents one square location on the board.*/
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

/** Represents one possible disk placement for given disk color.*/
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

/** Game settings.*/
data class Settings(
    val boardSize: Int,
    val autoplayMode: Boolean,
    val useDefaults: Boolean,
    val showHelpers: Boolean,
    val showLog: Boolean,
    val testMode: Boolean,
)

/** Player settings.*/
data class PlayerSettings(val showHelpers: Boolean, val testMode: Boolean)

/** Get player setting values from overall game settings.*/
fun Settings.toPlayerSettings(): PlayerSettings = PlayerSettings(
    showHelpers = this.showHelpers,
    testMode = this.testMode,
)

/** Returns the print colour for the given Disk.*/
fun Disk.diskColor(): AnsiColor = when (this) {
    Disk.Empty -> AnsiColor.WHITE
    Disk.White -> AnsiColor.CYAN
    Disk.Black -> AnsiColor.MAGENTA
}

/** Returns string character representing board status (black, white, empty).*/
fun Disk.boardChar(color: Boolean = true): String = when (this) {
    Disk.Empty -> "_"
    Disk.White -> if (color) getColor("W", this.diskColor()) else "W"
    Disk.Black -> if (color) getColor("B", this.diskColor()) else "B"
}

/** Returns the disk formatted as a coloured string.*/
fun Disk.name(): String = getColor(this.name.uppercase(), this.diskColor())

/** Returns the opposing disk colour.*/
fun Disk.opponent(): Disk = when (this) {
    Disk.Empty -> Disk.Empty
    Disk.White -> Disk.Black
    Disk.Black -> Disk.White
}

/** Calculate SHA256 hash for the given string.*/
fun calculateSha256(input: String): String {
    val buffer = Buffer()
    val hashingSink = HashingSink.sha256(buffer)
    val bufferedSink = hashingSink.buffer()

    bufferedSink.use { sink ->
        sink.writeUtf8(input)
    }

    return hashingSink.hash.hex()
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

fun versionInfo(): String =
    "${BuildInfo.version} ${BuildInfo.date} ${BuildInfo.branch} ${BuildInfo.commit}"
