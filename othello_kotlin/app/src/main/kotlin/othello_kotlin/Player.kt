package othello_kotlin

import java.awt.Color
import java.util.Random
import kotlin.system.exitProcess

class Player(private val disk: Disk) {
    var canPlay = true
    private var isHuman = true
    private var roundsPlayed = 0
    private val random = Random()
    private val showHelpers = true

    fun playOneMove(board: Board) {
        println("Turn: ${disk.name()}")
        val moves = board.possibleMoves(disk)

        if (moves.isNotEmpty()) {
            canPlay = true

            if (isHuman && showHelpers) {
                board.printMoves(moves)
            }

            val chosenMove = if (isHuman) getHumanMove(moves) else getComputerMove(moves)
            board.placeDisk(chosenMove)
            board.printScore()
            roundsPlayed++
            Thread.sleep(1000)
        } else {
            canPlay = false
            writeLine("  No moves available...", Color.YELLOW)
        }
    }

    fun setHuman(isHuman: Boolean) {
        this.isHuman = isHuman
    }

    override fun toString(): String {
        return "${disk.name()} | ${typeString()} | Moves: $roundsPlayed"
    }

    fun reset() {
        roundsPlayed = 0
        canPlay = true
    }

    private fun getComputerMove(moves: List<Move>): Move {
        println("  Computer plays...")
        Thread.sleep((random.nextInt(1000) + 1000).toLong())
        val chosenMove = moves[random.nextInt(moves.size)]
        println("  ${chosenMove.square} -> ${chosenMove.value}")
        return chosenMove
    }

    private fun getHumanMove(moves: List<Move>): Move {
        while (true) {
            val square = getSquare()
            val validMove = moves.find { it.square == square }
            if (validMove != null) {
                return validMove
            }
            error("can't place a ${disk.name()} disk in square $square!", 2)
        }
    }

    private fun getSquare(): Square {
        while (true) {
            try {
                print("  Give disk position (x,y): ")
                val coords = readLine() ?: exitProcess(1)

                if (coords.length != 3 || coords[1] != ',') {
                    throw IllegalArgumentException()
                }

                val x = coords.substring(0, 1).toInt()
                val y = coords.substring(2, 3).toInt()

                return Square(x, y)
            } catch (e: IllegalArgumentException) {
                error("give coordinates in the form (x,y)!", 2)
            }
        }
    }

    private fun typeString(): String {
        return if (isHuman) "Human   " else "Computer"
    }
}
