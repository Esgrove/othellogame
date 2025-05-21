package othello;

public enum AnsiColor {
    CYAN("\u001B[36m"),
    GREEN("\u001B[32m"),
    MAGENTA("\u001B[35m"),
    RED("\u001B[31m"),
    WHITE("\u001B[37m"),
    YELLOW("\u001B[33m"),
    BOLD("\u001B[1m"),
    RESET("\u001B[0m");

    private final String code;

    AnsiColor(String code) {
        this.code = code;
    }

    @Override
    public String toString() {
        return code;
    }
}
