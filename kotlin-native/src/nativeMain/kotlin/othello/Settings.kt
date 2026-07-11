package othello

/** Minimum allowed board size. */
const val MIN_BOARD_SIZE = 4

/** Maximum allowed board size. */
const val MAX_BOARD_SIZE = 10

/** Default board size when none is given. */
const val DEFAULT_BOARD_SIZE = 8

/** Game settings. */
data class Settings(
    val boardSize: Int,
    val autoplayMode: Boolean,
    val checkMode: Boolean,
    val showHelpers: Boolean,
    val showLog: Boolean,
    val testMode: Boolean,
    val useDefaults: Boolean,
) {
    companion object {
        /** Get default game settings. */
        fun default(): Settings = Settings(
            boardSize = DEFAULT_BOARD_SIZE,
            autoplayMode = false,
            checkMode = false,
            showHelpers = true,
            showLog = false,
            testMode = false,
            useDefaults = false,
        )
    }

    override fun toString(): String = """
        |Settings:
        |  board_size: $boardSize
        |  autoplay: $autoplayMode
        |  check_mode: $checkMode
        |  use_defaults: $useDefaults
        |  show_helpers: $showHelpers
        |  show_log: $showLog
        |  test_mode: $testMode
    """.trimMargin()
}

/** Player settings. */
data class PlayerSettings(
    val showHelpers: Boolean,
    val checkMode: Boolean,
    val testMode: Boolean,
) {
    companion object {
        /** Get default player settings. */
        fun default(): PlayerSettings = PlayerSettings(
            showHelpers = true,
            checkMode = false,
            testMode = false,
        )
    }
}

/** Get player setting values from overall game settings. */
fun Settings.toPlayerSettings(): PlayerSettings = PlayerSettings(
    showHelpers = this.showHelpers,
    checkMode = this.checkMode,
    testMode = this.testMode,
)
