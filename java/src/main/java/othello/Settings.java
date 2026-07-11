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

    /** Minimum allowed board size. */
    public static final int MIN_BOARD_SIZE = 4;
    /** Maximum allowed board size. */
    public static final int MAX_BOARD_SIZE = 10;
    /** Default board size when none is given. */
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
        return """
            Settings:
              board_size: %d
              autoplay: %b
              check_mode: %b
              use_defaults: %b
              show_helpers: %b
              show_log: %b
              test_mode: %b"""
            .formatted(boardSize, autoplayMode, checkMode, useDefaults, showHelpers, showLog, testMode);
    }
}
