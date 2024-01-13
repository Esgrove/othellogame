package othello

import kotlin.random.Random

/** Defines one player that can be either human or computer controlled.*/
class Player(private val disk: Disk, private val settings: PlayerSettings) {
    var canPlay = true
    private var isHuman = true
    private var roundsPlayed = 0
    private val random = Random

    companion object {
        fun black(settings: PlayerSettings): Player {
            return Player(Disk.Black, settings)
        }

        fun white(settings: PlayerSettings): Player {
            return Player(Disk.White, settings)
        }
    }

    /** Play one round as this player.*/
    fun playOneMove(board: Board): String? {
        println("Turn: ${disk.name()}")
        val moves = board.possibleMoves(disk)
        if (moves.isNotEmpty()) {
            canPlay = true
            if (isHuman && settings.showHelpers) {
                board.printPossibleMoves(moves)
            }
            val chosenMove = if (isHuman) getHumanMove(moves) else getComputerMove(moves)
            board.placeDisk(chosenMove)
            board.printScore()
            roundsPlayed++
            if (!settings.testMode) {
                Thread.sleep(1000)
            }
            return chosenMove.toLogEntry()
        }
        canPlay = false
        printColor("  No moves available...", AnsiColor.YELLOW)
        return null
    }

    /** Reset player status for a new game.*/
    fun reset() {
        roundsPlayed = 0
        canPlay = true
    }

    /** Set the player as human or computer controlled.*/
    fun setHuman(isHuman: Boolean) {
        this.isHuman = isHuman
    }

    /** Return move chosen by computer.*/
    private fun getComputerMove(moves: List<Move>): Move {
        println("  Computer plays...")
        val chosenMove = if (settings.testMode) {
            moves[0]
        } else {
            Thread.sleep((random.nextInt(1000) + 1000).toLong())
            moves[random.nextInt(moves.size)]
        }

        println("  ${chosenMove.square} -> ${chosenMove.value}")
        return chosenMove
    }

    /** Return move chosen by a human player.*/
    private fun getHumanMove(moves: List<Move>): Move {
        while (true) {
            val square = getSquare()
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

                val x = coordinates.substring(0, 1).toInt()
                val y = coordinates.substring(2, 3).toInt()

                return Square(x, y)
            } catch (e: IllegalArgumentException) {
                printError("  Give coordinates in the form 'x,y'")
            }
        }
    }

    /** Return player type description string.*/
    private fun typeString(): String {
        return if (isHuman) "Human   " else "Computer"
    }

    override fun toString(): String {
        return "${disk.name()} | ${typeString()} | Moves: $roundsPlayed"
    }
}
