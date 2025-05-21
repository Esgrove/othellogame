package othello;

/**
 * Game settings.
 */
public class Settings {
    private final int boardSize;
    private final boolean autoplayMode;
    private final boolean checkMode;
    private final boolean showHelpers;
    private final boolean showLog;
    private final boolean testMode;
    private final boolean useDefaults;

    public Settings(
        int boardSize,
        boolean autoplayMode,
        boolean checkMode,
        boolean showHelpers,
        boolean showLog,
        boolean testMode,
        boolean useDefaults
    ) {
        this.boardSize = boardSize;
        this.autoplayMode = autoplayMode;
        this.checkMode = checkMode;
        this.showHelpers = showHelpers;
        this.showLog = showLog;
        this.testMode = testMode;
        this.useDefaults = useDefaults;
    }

    public int getBoardSize() {
        return boardSize;
    }

    public boolean autoplayMode() {
        return autoplayMode;
    }

    public boolean useDefaults() {
        return useDefaults;
    }

    public boolean showHelpers() {
        return showHelpers;
    }

    public boolean showLog() {
        return showLog;
    }

    public boolean testMode() {
        return testMode;
    }

    public boolean checkMode() {
        return checkMode;
    }

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
