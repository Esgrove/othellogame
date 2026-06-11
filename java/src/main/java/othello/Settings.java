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

    /**
     * Get player setting values from overall game settings.
     */
    public PlayerSettings toPlayerSettings() {
        return new PlayerSettings(showHelpers, checkMode, testMode);
    }

    /**
     * Get default settings.
     */
    public static Settings defaultSettings() {
        return new Settings(DEFAULT_BOARD_SIZE, false, false, true, false, false, false);
    }

    @Override
    public String toString() {
        return "Settings:"
            + "\n  board_size: " + boardSize
            + "\n  autoplay: " + autoplayMode
            + "\n  check_mode: " + checkMode
            + "\n  use_defaults: " + useDefaults
            + "\n  show_helpers: " + showHelpers
            + "\n  show_log: " + showLog
            + "\n  test_mode: " + testMode;
    }
}
