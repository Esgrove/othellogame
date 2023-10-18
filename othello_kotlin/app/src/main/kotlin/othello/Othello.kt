package othello

import java.awt.Color
import kotlin.system.exitProcess

/** Gameplay loop and main logic.*/
class Othello(private val settings: Settings) {
    private var board = Board(settings.boardSize)
    private val playerBlack = Player.black(settings.toPlayerSettings())
    private val playerWhite = Player.white(settings.toPlayerSettings())
    private var roundsPlayed = 0
    private var gamesPlayed = 0
    private var gameLog = mutableListOf<String>()

    /** Play one full game of Othello.*/
    fun play() {
        while (true) {
            initGame()
            gameLoop()
            printResult()
            if (settings.showLog) {
                printLog()
            }
            if (settings.autoplayMode || !getAnswer("Would you like to play again")) {
                break
            }
        }
    }

    /** Initialize game board and players for a new game.*/
    private fun initGame() {
        if (gamesPlayed > 0) {
            board = Board(settings.boardSize)
            playerBlack.reset()
            playerWhite.reset()
            roundsPlayed = 0
            gameLog.clear()
        }

        if (settings.autoplayMode) {
            // Computer plays both
            playerBlack.setHuman(false)
            playerWhite.setHuman(false)
        } else if (settings.quickStart) {
            // Default: play as black against white computer player
            playerWhite.setHuman(false)
        } else if (getAnswer("Would you like to play against the computer")) {
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
            for (player in listOf(playerBlack, playerWhite)) {
                val result = player.playOneMove(board)
                if (result != null) {
                    gameLog.add("$result;${board.toLogEntry()}")
                }
            }
            println("--------------------------------")
        }
        gamesPlayed++
    }

    private fun printLog() {
        val formattedLog = gameLog
            .mapIndexed { index, line -> String.format("%02d: %s", index + 1, line) }
            .joinToString("\n")

        val hexHash = calculateSha256(formattedLog)

        printBold("Game log:", Color.YELLOW)
        println(formattedLog)
        println(hexHash)
    }

    /** Print ending status and winner info.*/
    private fun printResult() {
        printBold("\n================================")
        printColor("The game is finished!", Color.GREEN)
        printBold("Result:")
        printStatus()
        println()

        val winner = board.result()
        if (winner == Disk.Empty) {
            printBold("The game ended in a tie...\n")
        } else {
            printBold("The winner is ${winner.name()}!\n")
        }
    }

    /** Print current board and player info.*/
    private fun printStatus() {
        println(playerBlack)
        println(playerWhite)
        println("\n$board")
    }

    /** Ask a question with two options, and return bool from user answer.*/
    private fun getAnswer(question: String, yes: String = "y", no: String = "n"): Boolean {
        print("$question ($yes/$no)? ")
        val ans = readlnOrNull()
        return !ans.isNullOrBlank() && ans.equals(yes, ignoreCase = true)
    }
}

fun main(args: Array<String>) {
    printBold("OTHELLO GAME - KOTLIN", Color.GREEN)

    if (args.contains("--help") || args.contains("-h")) {
        println("Othello Kotlin ${versionInfo()}\n")
        println("USAGE: java -jar othello_kotlin.jar [board size]\n")
        println("Optional arguments:")
        println("    -h | --help          Print usage and exit")
        println("    -v | --version       Print version info and exit")
        exitProcess(1)
    }

    if (args.contains("--version") || args.contains("-v")) {
        println("Othello Kotlin ${versionInfo()}")
        exitProcess(0)
    }

    // Try to read board size from command line args
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

    val settings = Settings(
        boardSize,
        autoplayMode = true,
        quickStart = false,
        showHelpers = true,
        showLog = true,
        testMode = true,
    )

    val game = Othello(settings)
    game.play()
}

/** Ask and return the desired board size.*/
private fun getBoardSize(): Int {
    print("Choose board size (default is $DEFAULT_BOARD_SIZE): ")
    val input = readlnOrNull()
    if (input != null) {
        val boardSize = input.toIntOrNull()
        if (boardSize != null) {
            if (boardSize !in MIN_BOARD_SIZE..MAX_BOARD_SIZE) {
                printWarn("Limiting board size to valid range $MIN_BOARD_SIZE...$MAX_BOARD_SIZE")
            }
            return boardSize.coerceIn(MIN_BOARD_SIZE, MAX_BOARD_SIZE)
        }
    }
    printWarn("Invalid size, defaulting to $DEFAULT_BOARD_SIZE...")
    return DEFAULT_BOARD_SIZE
}
