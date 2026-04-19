package othello;

/**
 * Game settings.
 */
public record Settings(
    int boardSize,
    boolean autoplayMode,
    boolean checkMode,
    boolean showHelpers,
    boolean showLog,
    boolean testMode,
    boolean useDefaults
) {

    public static final int MIN_BOARD_SIZE = 4;
    public static final int MAX_BOARD_SIZE = 10;
    public static final int DEFAULT_BOARD_SIZE = 8;

    public PlayerSettings toPlayerSettings() {
        return new PlayerSettings(this.checkMode, this.showHelpers, this.testMode);
    }

    @Override
    public String toString() {
        return String.format(
            """
                Settings:
                  boardSize: %d
                  autoplayMode: %b
                  checkMode: %b
                  showHelpers: %b
                  showLog: %b
                  testMode: %b
                  useDefaults: %b
                """,
            boardSize,
            autoplayMode,
            checkMode,
            useDefaults,
            showHelpers,
            showLog,
            testMode
        );
    }
}
