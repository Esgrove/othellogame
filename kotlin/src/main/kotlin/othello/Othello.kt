package othello

/** Gameplay loop and main logic. */
class Othello(private val settings: Settings) {
    private var board = Board(settings.boardSize)
    private val playerBlack = Player.black(settings.toPlayerSettings())
    private val playerWhite = Player.white(settings.toPlayerSettings())
    private var gameLog = mutableListOf<String>()
    private var gamesPlayed = 0
    private var roundsPlayed = 0

    /** Play one full game of Othello. */
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

    /** Initialize game board and players for a new game. */
    private fun initGame() {
        // Re-use existing objects instead of initializing new ones
        if (gamesPlayed > 0) {
            board = Board(settings.boardSize)
            playerBlack.reset()
            playerWhite.reset()
            roundsPlayed = 0
            gameLog.clear()
        }
        if (settings.autoplayMode) {
            // Computer plays both
            playerBlack.setComputer()
            playerWhite.setComputer()
        } else if (settings.useDefaults) {
            // Default: play as black against white computer player
            playerWhite.setComputer()
        } else if (getAnswer("Would you like to play against the computer")) {
            if (getAnswer("Would you like to play as black or white", "b", "w")) {
                playerWhite.setComputer()
            } else {
                playerBlack.setComputer()
            }
        }
        if (!settings.checkMode) {
            printBold("\nPlayers:")
            printStatus()
        }
    }

    /** Keep making moves until both players can't make a move any more. */
    private fun gameLoop() {
        while (board.canPlay() && (playerBlack.canPlay || playerWhite.canPlay)) {
            roundsPlayed++
            printRoundHeader()
            for (player in listOf(playerBlack, playerWhite)) {
                val result = player.playOneMove(board)
                if (result != null) {
                    gameLog.add("$result;${board.logEntry()}")
                }
                if (!settings.checkMode) {
                    println("--------------------------------")
                }
            }
        }
        gamesPlayed++
        printGameEndFooter()
    }

    private fun formatGameLog(): String = gameLog
        .mapIndexed { index, line -> "${(index + 1).toString().padStart(2, '0')}: $line" }
        .joinToString("\n")

    private fun printRoundHeader() {
        if (!settings.checkMode) {
            printBold("\n=========== ROUND: $roundsPlayed ===========")
        }
    }

    private fun printGameEndFooter() {
        if (!settings.checkMode) {
            printBold("\n================================")
            printBold("The game is finished!\n", AnsiColor.GREEN)
        }
    }

    /** Print game log which shows all moves made and the game board state after each move. */
    private fun printLog() {
        val formattedLog = formatGameLog()
        if (!settings.checkMode) {
            printBold("Game log:", AnsiColor.YELLOW)
            println(formattedLog)
        }
        val hexHash = calculateSha256(formattedLog)
        println(hexHash)
    }

    /** Print ending status and winner info. */
    private fun printResult() {
        printBold("Result:")
        printStatus()
        println()

        val winner = board.result()
        if (winner == Disk.Empty) {
            println("The game ended in a tie...\n")
        } else {
            println("The winner is ${winner.diskString()}!\n")
        }
    }

    /** Print current board and player info. */
    private fun printStatus() {
        println(playerBlack)
        println("$playerWhite\n")
        println(board)
    }

    /** Ask a question with two options, and return bool from user answer. */
    private fun getAnswer(question: String, yes: String = "y", no: String = "n"): Boolean {
        print("$question ($yes/$no)? ")
        val input = readlnOrNull() ?: return false
        return input.trim().equals(yes, ignoreCase = true)
    }

    companion object {
        /** Ask and return the desired board size. */
        fun getBoardSize(): Int {
            print("Choose board size (default is $DEFAULT_BOARD_SIZE): ")
            val boardSize = readlnOrNull()?.trim()?.toIntOrNull()
            if (boardSize != null) {
                if (boardSize !in MIN_BOARD_SIZE..MAX_BOARD_SIZE) {
                    printColor(
                        "Limiting board size to valid range $MIN_BOARD_SIZE..$MAX_BOARD_SIZE",
                        AnsiColor.YELLOW,
                    )
                }
                return boardSize.coerceIn(MIN_BOARD_SIZE, MAX_BOARD_SIZE)
            }
            printWarn("Invalid size, defaulting to $DEFAULT_BOARD_SIZE...")
            return DEFAULT_BOARD_SIZE
        }
    }
}
