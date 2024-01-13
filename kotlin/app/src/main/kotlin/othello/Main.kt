package othello

import com.github.ajalt.clikt.core.CliktCommand
import com.github.ajalt.clikt.parameters.arguments.argument
import com.github.ajalt.clikt.parameters.arguments.optional
import com.github.ajalt.clikt.parameters.options.flag
import com.github.ajalt.clikt.parameters.options.option
import com.github.ajalt.clikt.parameters.types.int
import kotlin.system.exitProcess

class OthelloKotlin : CliktCommand(
    help = "A simple Othello CLI game implementation.\n\njava -jar othello_kotlin.jar",
) {
    private val size by argument(
        "size",
        help = "Optional board size ($MIN_BOARD_SIZE..$MAX_BOARD_SIZE)",
    ).int().optional()
    private val autoplay by option("-a", "--autoplay", help = "Enable autoplay mode").flag()
    private val default by option("-d", "--default", help = "Play with default settings").flag()
    private val log by option("-l", "--log", help = "Show log after a game").flag()
    private val noHelpers by option("-n", "--no-helpers", help = "Hide disk placement hints").flag()
    private val test by option("-t", "--test", help = "Enable test mode").flag()
    private val version by option("-v", "--version", help = "Print version and exit").flag()

    override fun run() {
        if (version) {
            println("Othello Kotlin ${versionInfo()}")
            exitProcess(0)
        }

        // Try to read board size from command line args
        val boardSize = if (size != null) {
            if (size!! < MIN_BOARD_SIZE || size!! > MAX_BOARD_SIZE) {
                printError("Unsupported board size: $size")
                exitProcess(1)
            }
            println("Using board size: $size")
            size!!
        } else if (autoplay || default) {
            DEFAULT_BOARD_SIZE
        } else {
            getBoardSize()
        }

        val settings = Settings(
            boardSize,
            autoplayMode = autoplay,
            useDefaults = default,
            showHelpers = !noHelpers,
            showLog = log,
            testMode = test,
        )

        val game = Othello(settings)
        game.play()
    }
}

fun main(args: Array<String>) {
    printBold("OTHELLO GAME - KOTLIN", AnsiColor.GREEN)
    OthelloKotlin().main(args)
}
