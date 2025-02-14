package othello

/** Game settings.*/
data class Settings(
    val boardSize: Int,
    val autoplayMode: Boolean,
    val checkMode: Boolean,
    val showHelpers: Boolean,
    val showLog: Boolean,
    val testMode: Boolean,
    val useDefaults: Boolean,
) {
    override fun toString(): String = """
            Settings:
              boardSize: $boardSize
              autoplayMode: $autoplayMode
              checkMode: $checkMode
              showHelpers: $showHelpers
              showLog: $showLog
              testMode: $testMode
              useDefaults: $useDefaults
    """.trimIndent()
}

/** Player settings.*/
data class PlayerSettings(val checkMode: Boolean, val showHelpers: Boolean, val testMode: Boolean)

/** Get player setting values from overall game settings.*/
fun Settings.toPlayerSettings(): PlayerSettings = PlayerSettings(
    checkMode = this.checkMode,
    showHelpers = this.showHelpers,
    testMode = this.testMode,
)
