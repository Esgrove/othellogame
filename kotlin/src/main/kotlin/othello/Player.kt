package othello

import kotlin.random.Random

/** Defines one player that can be either human or computer controlled.*/
class Player(internal val disk: Disk, internal val settings: PlayerSettings) {
    internal var canPlay = true
    internal var isHuman = true
    internal var roundsPlayed = 0
    private val random = Random

    companion object {
        fun black(settings: PlayerSettings): Player = Player(Disk.Black, settings)

        fun white(settings: PlayerSettings): Player = Player(Disk.White, settings)

        fun default(): Player = Player(
            disk = Disk.Black,
            settings = PlayerSettings.default(),
        )
    }

    /** Play one round as this player.*/
    fun playOneMove(board: Board): String? {
        if (!settings.checkMode) {
            println("Turn: ${disk.name()}")
        }
        val moves = board.possibleMoves(disk)
        if (moves.isNotEmpty()) {
            canPlay = true
            if (isHuman && settings.showHelpers && !settings.checkMode) {
                board.printPossibleMoves(moves)
            }
            val chosenMove = if (isHuman) getHumanMove(moves) else getComputerMove(moves)
            board.placeDisk(chosenMove)
            if (!settings.checkMode) {
                board.printScore()
            }
            roundsPlayed++
            if (!settings.testMode) {
                Thread.sleep(1000)
            }
            return chosenMove.logEntry()
        }
        canPlay = false
        if (!settings.checkMode) {
            printColor("  No moves available...", AnsiColor.YELLOW)
        }
        return null
    }

    /** Reset player status for a new game.*/
    fun reset() {
        roundsPlayed = 0
        canPlay = true
    }

    /** Set the player as human controlled.*/
    fun setHuman() {
        this.isHuman = true
    }

    /** Set the player as computer controlled.*/
    fun setComputer() {
        this.isHuman = false
    }

    /** Return move chosen by computer.*/
    private fun getComputerMove(moves: List<Move>): Move {
        if (!settings.checkMode) {
            println("  Computer plays...")
        }
        val chosenMove = if (settings.testMode) {
            moves[0]
        } else {
            // Wait a bit and pick a random move
            Thread.sleep((random.nextInt(1000) + 1000).toLong())
            moves[random.nextInt(moves.size)]
        }
        if (!settings.checkMode) {
            println("  ${chosenMove.square} -> ${chosenMove.value}")
        }
        return chosenMove
    }

    /** Return move chosen by a human player.*/
    private fun getHumanMove(moves: List<Move>): Move {
        while (true) {
            val square = getSquare()
            // Check that the chosen square is actually one of the possible moves
            val validMove = moves.find { it.square == square }
            if (validMove != null) {
                return validMove
            }
            printError("  Can't place a ${disk.name()} disk in square $square")
        }
    }

    /** Ask human player for square coordinates.*/
    private fun getSquare(): Square {
        while (true) {
            try {
                print("  Give disk position (x,y): ")
                val coordinates = readlnOrNull() ?: ""

                if (coordinates.length != 3 || coordinates[1] != ',') {
                    throw IllegalArgumentException()
                }

                val x = coordinates[0].digitToInt()
                val y = coordinates[2].digitToInt()

                return Square(x, y)
            } catch (_: IllegalArgumentException) {
                printError("  Give coordinates in the form 'x,y'")
            }
        }
    }

    /** Return player type description string.*/
    internal fun typeString(): String = if (isHuman) "Human   " else "Computer"

    override fun toString(): String = "${disk.name()} | ${typeString()} | Moves: $roundsPlayed"
}
