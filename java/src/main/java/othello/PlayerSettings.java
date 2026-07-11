package othello;

/**
 * Player settings.
 */
public record PlayerSettings(boolean showHelpers, boolean checkMode, boolean testMode) {

    /**
     * Get default player settings.
     */
    public static PlayerSettings defaultSettings() {
        return new PlayerSettings(true, false, false);
    }
}
