package othello_kotlin

import othello_kotlin.*
import java.awt.Color
import java.util.*
import kotlin.error
import kotlin.system.exitProcess

object OthelloGame {
    private const val MIN_BOARD_SIZE = 4
    private const val MAX_BOARD_SIZE = 10

    @JvmStatic
    fun main(args: Array<String>) {
        printColor("OTHELLO GAME - Kotlin", Color.GREEN)

        if (args.contains("--help") || args.contains("-h")) {
            println("othello_kotlin.Othello Kotlin ${getCurrentDateTime()}\n")
            println("USAGE: othello.jar [board size]\n")
            println("Optional arguments:")
            println("    -h | --help          Print usage and exit")
            println("    -v | --version       Print version info and exit")
            exitProcess(1)
        }

        if (args.contains("--version") || args.contains("-v")) {
            println("othello_kotlin.Othello Kotlin ${getCurrentDateTime()}")
            exitProcess(0)
        }

        val boardSize = if (args.isEmpty() || args[0].toIntOrNull() == null) {
            getBoardSize()
        } else {
            val size = args[0].toInt()
            if (size < MIN_BOARD_SIZE || size > MAX_BOARD_SIZE) {
                printError("Unsupported board size: $size")
            }
            println("Using board size: $size")
            size
        }

        val game = Othello(boardSize)
        game.play()
    }

    private fun getCurrentDateTime(): String {
        val now = Calendar.getInstance()
        return now.time.toString()
    }

    /** */
    private fun getBoardSize(): Int {
        while (true) {
            print("Choose board size (default is 8): ")
            val input = readlnOrNull()
            if (input != null) {
                val boardSize = input.toIntOrNull()
                if (boardSize != null && boardSize in MIN_BOARD_SIZE..MAX_BOARD_SIZE) {
                    return boardSize
                }
            }
            error("Give a valid number...")
        }
    }
}

/** Othello Gameplay loop and main logic.*/
class Othello(private val boardSize: Int) {
    private var board: Board = Board(boardSize)
    private val playerBlack: Player = Player(Disk.Black)
    private val playerWhite: Player = Player(Disk.White)
    private var roundsPlayed = 0
    private var gamesPlayed = 0

    init {
        gamesPlayed = 0
        roundsPlayed = 0
    }

    /** */
    fun play() {
        while (true) {
            initGame()
            gameLoop()
            printResult()
            if (!getAnswer("\nWould you like to play again")) {
                break
            }
        }
    }

    /** Initialize game board and players for a new game.*/
    private fun initGame() {
        if (gamesPlayed > 0) {
            board = Board(boardSize)
            playerBlack.reset()
            playerWhite.reset()
            roundsPlayed = 0
        }

        if (getAnswer("Would you like to play against the computer")) {
            if (getAnswer("Would you like to play as black or white", "b", "w")) {
                playerWhite.setHuman(false)
            } else {
                playerBlack.setHuman(false)
            }
        }

        printBold("\nPlayers:")
        printStatus()
    }

    /** Keep making moves until both players can't make a move any more.*/
    private fun gameLoop() {
        while (board.canPlay() && (playerBlack.canPlay || playerWhite.canPlay)) {
            roundsPlayed++
            printBold("\n=========== ROUND: $roundsPlayed ===========")
            playerBlack.playOneMove(board)
            println("--------------------------------")
            playerWhite.playOneMove(board)
        }
        gamesPlayed++
    }

    /** */
    private fun printResult() {
        printBold("\n================================")
        printColor("The game is finished!", Color.GREEN)
        printBold("Result:")
        printStatus()
        println()

        val winner = board.result()
        if (winner == Disk.Empty) {
            printBold("The game ended in a tie...")
        } else {
            printBold("The winner is ${winner.name()}!")
        }
    }

    /** */
    private fun printStatus() {
        println(playerBlack)
        println(playerWhite)
        println("\n$board")
    }

    /** */
    private fun getAnswer(question: String, yes: String = "y", no: String = "n"): Boolean {
        print("$question ($yes/$no)? ")
        val ans = readlnOrNull()
        return !ans.isNullOrBlank() && ans.equals(yes, ignoreCase = true)
    }
}
