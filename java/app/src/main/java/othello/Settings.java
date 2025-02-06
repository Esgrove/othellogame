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

    public Settings(int boardSize, boolean autoplayMode, boolean checkMode, boolean useDefaults, boolean showHelpers, boolean showLog, boolean testMode) {
        this.boardSize = boardSize;
        this.autoplayMode = autoplayMode;
        this.checkMode = checkMode;
        this.useDefaults = useDefaults;
        this.showHelpers = showHelpers;
        this.showLog = showLog;
        this.testMode = testMode;
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
                  useDefaults: %b
                  showHelpers: %b
                  showLog: %b
                  testMode: %b
                """,
            boardSize, autoplayMode, checkMode, useDefaults, showHelpers, showLog, testMode
        );
    }
}
