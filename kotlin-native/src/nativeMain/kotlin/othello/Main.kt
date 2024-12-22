package othello

import com.github.ajalt.clikt.core.CliktCommand
import com.github.ajalt.clikt.core.Context
import com.github.ajalt.clikt.core.main
import com.github.ajalt.clikt.parameters.arguments.argument
import com.github.ajalt.clikt.parameters.arguments.optional
import com.github.ajalt.clikt.parameters.options.flag
import com.github.ajalt.clikt.parameters.options.option
import com.github.ajalt.clikt.parameters.types.int
import kotlin.system.exitProcess

class OthelloKotlin : CliktCommand("othello_kotlin") {
    private val size by argument(
        "size",
        help = "Optional board size ($MIN_BOARD_SIZE..$MAX_BOARD_SIZE)",
    ).int().optional()

    @Suppress("ktlint")
    private val autoplay by option("-a", "--autoplay", help = "Enable autoplay mode with computer control").flag()
    private val check by option("-c", "--check", help = "Autoplay and only print result").flag()
    private val default by option("-d", "--default", help = "Play with default settings").flag()
    private val log by option("-l", "--log", help = "Show game log at the end").flag()
    private val noHelpers by option("-n", "--no-helpers", help = "Hide disk placement hints").flag()
    @Suppress("ktlint")
    private val test by option("-t", "--test", help = "Enable test mode with deterministic computer moves").flag()
    private val version by option("-v", "--version", help = "Print version and exit").flag()

    override fun help(context: Context) =
        "A simple Othello CLI game implementation in Kotlin\n\njava -jar othello_kotlin.jar"

    override fun run() {
        if (version) {
            println(VersionInfo.VERSION_STRING)
            exitProcess(0)
        }

        printBold("OTHELLO GAME - KOTLIN", AnsiColor.GREEN)

        val boardSize = resolveBoardSize()

        val settings = Settings(
            boardSize,
            autoplayMode = autoplay || check,
            checkMode = check,
            showHelpers = !noHelpers,
            showLog = log || check,
            testMode = test || check,
            useDefaults = default,
        )

        Othello(settings).play()
    }

    private fun resolveBoardSize(): Int {
        // Try to read board size from command line args
        if (size != null) {
            val boardSize = size!!
            if (boardSize !in MIN_BOARD_SIZE..MAX_BOARD_SIZE) {
                printError("Unsupported board size: $boardSize")
                exitProcess(1)
            }
            println("Using board size: $boardSize")
            return boardSize
        } else if (autoplay || default) {
            return DEFAULT_BOARD_SIZE
        } else {
            return getBoardSize()
        }
    }
}

fun main(args: Array<String>) {
    OthelloKotlin().main(args)
}
