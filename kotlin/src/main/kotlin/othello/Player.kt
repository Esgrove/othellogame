package othello

import kotlin.random.Random

/**
 * Defines one player that can be either human or computer controlled.
 *
 * @constructor Initialize new player for the given disk color.
 */
class Player(internal val disk: Disk, internal val settings: PlayerSettings) {
    internal var canPlay = true
    internal var playerType = PlayerType.Human
    internal var roundsPlayed = 0
    private val random = Random

    companion object {
        /** Shorthand to initialize a new player for black disks. */
        fun black(settings: PlayerSettings): Player = Player(Disk.Black, settings)

        /** Shorthand to initialize a new player for white disks. */
        fun white(settings: PlayerSettings): Player = Player(Disk.White, settings)

        /** Get a default player. */
        fun default(): Player = Player(Disk.Black, PlayerSettings.default())
    }

    /** Play one round as this player. */
    fun playOneMove(board: Board): String? {
        if (!settings.checkMode) {
            println("Turn: ${disk.diskString()}")
        }
        val moves = board.possibleMoves(disk)
        if (moves.isEmpty()) {
            canPlay = false
            if (!settings.checkMode) {
                printYellow("  No moves available...")
            }
            return null
        }
        canPlay = true
        if (human() && settings.showHelpers && !settings.checkMode) {
            board.printPossibleMoves(moves)
        }
        val chosenMove = if (human()) getHumanMove(moves) else getComputerMove(moves)
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

    /** Reset player status for a new game. */
    fun reset() {
        canPlay = true
        roundsPlayed = 0
    }

    /** Returns true if player is controlled by a human player. */
    fun human(): Boolean = playerType.human()

    /** Returns true if player is controlled by computer. */
    fun computer(): Boolean = playerType.computer()

    /** Set the player as human or computer controlled. */
    fun setPlayerType(playerType: PlayerType) {
        this.playerType = playerType
    }

    /** Set the player as human controlled. */
    fun setHuman() {
        setPlayerType(PlayerType.Human)
    }

    /** Set the player as computer controlled. */
    fun setComputer() {
        setPlayerType(PlayerType.Computer)
    }

    /** Return move chosen by computer. */
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

    /** Return move chosen by a human player. */
    private fun getHumanMove(moves: List<Move>): Move {
        while (true) {
            val square = getSquare()
            // Check that the chosen square is actually one of the possible moves
            val validMove = moves.find { it.square == square }
            if (validMove != null) {
                return validMove
            }
            printError("  Can't place a ${disk.diskString()} disk in square $square!")
        }
    }

    /** Ask human player for square coordinates. */
    private fun getSquare(): Square {
        while (true) {
            print("  Give disk position (x,y): ")
            val input = readlnOrNull()
            if (input == null) {
                printError("  Input failed. Please try again.")
                continue
            }
            val values = input.trim().split(',')
            if (values.size == 2) {
                val x = values[0].toIntOrNull() ?: -1
                val y = values[1].toIntOrNull() ?: -1
                if (x >= 0 && y >= 0) {
                    return Square(x, y)
                }
            }
            printError("  Give coordinates in the form 'x,y'!")
        }
    }

    /** Return player type description string. */
    internal fun typeString(): String = playerType.toString()

    override fun toString(): String =
        "${disk.diskString()} | ${typeString()} | Moves: $roundsPlayed"
}

/** Player can be controlled either by a human or computer. */
enum class PlayerType {
    Human,
    Computer,
    ;

    /** Check if the player is controlled by a human. */
    fun human(): Boolean = this == Human

    /** Check if the player is controlled by a computer. */
    fun computer(): Boolean = this == Computer

    override fun toString(): String = if (this == Human) "Human   " else "Computer"
}
