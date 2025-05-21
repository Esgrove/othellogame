package othello;

/**
 * Player settings.
 */
public class PlayerSettings {
    private final boolean checkMode;
    private final boolean showHelpers;
    private final boolean testMode;

    public PlayerSettings(boolean checkMode, boolean showHelpers, boolean testMode) {
        this.checkMode = checkMode;
        this.showHelpers = showHelpers;
        this.testMode = testMode;
    }

    public boolean checkMode() {
        return checkMode;
    }

    public boolean showHelpers() {
        return showHelpers;
    }

    public boolean testMode() {
        return testMode;
    }

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
