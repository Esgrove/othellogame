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

    public boolean isAutoplayMode() {
        return autoplayMode;
    }

    public boolean isUseDefaults() {
        return useDefaults;
    }

    public boolean isShowHelpers() {
        return showHelpers;
    }

    public boolean isShowLog() {
        return showLog;
    }

    public boolean isTestMode() {
        return testMode;
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
