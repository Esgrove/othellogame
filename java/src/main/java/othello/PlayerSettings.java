package othello;

/**
 * Player settings.
 */
public record PlayerSettings(boolean checkMode, boolean showHelpers, boolean testMode) {

    @Override
    public String toString() {
        return String.format(
            "PlayerSettings(checkMode=%b, showHelpers=%b, testMode=%b)",
            checkMode,
            showHelpers,
            testMode
        );
    }
}
