package othello;

/**
 * Defines ANSI escape codes for adding colours to console output.
 */
public enum AnsiColor {
    CYAN(36),
    GREEN(32),
    MAGENTA(35),
    RED(31),
    WHITE(37),
    YELLOW(33),
    BOLD(1),
    RESET(0);

    private final int code;

    AnsiColor(int code) {
        this.code = code;
    }

    /**
     * Return the combined bold + colour escape code for this colour.
     */
    public String bold() {
        return "\u001B[1;" + code + "m";
    }

    @Override
    public String toString() {
        return "\u001B[" + code + "m";
    }
}
