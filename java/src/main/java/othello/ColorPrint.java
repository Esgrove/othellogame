package othello;

/**
 * Interface for coloured terminal printing.
 */
public class ColorPrint {
    /**
     * Format string with colour.
     */
    public static String getColor(String text, AnsiColor color) {
        return color + text + AnsiColor.RESET;
    }

    /**
     * Format string as bold.
     */
    public static String getBold(String text) {
        return AnsiColor.BOLD + text + AnsiColor.RESET.toString();
    }

    /**
     * Format string as bold with colour.
     */
    public static String getBold(String text, AnsiColor color) {
        return color.bold() + text + AnsiColor.RESET;
    }

    /**
     * Print text with the given colour.
     */
    public static void printColor(String text, AnsiColor color) {
        System.out.println(getColor(text, color));
    }

    /**
     * Print bold text.
     */
    public static void printBold(String text) {
        System.out.println(getBold(text));
    }

    /**
     * Print bold text with the given colour.
     */
    public static void printBold(String text, AnsiColor color) {
        System.out.println(getBold(text, color));
    }

    /**
     * Print error message with red colour.
     */
    public static void printError(String message) {
        String[] split = splitLeadingWhitespace(message);
        System.out.println(getColor(split[0] + "Error: " + split[1], AnsiColor.RED));
    }

    /**
     * Print warning message with yellow colour.
     */
    public static void printWarn(String message) {
        String[] split = splitLeadingWhitespace(message);
        System.out.println(getColor(split[0] + "Warning: " + split[1], AnsiColor.YELLOW));
    }

    /**
     * Split a string into the leading whitespace and the rest of the string.
     */
    private static String[] splitLeadingWhitespace(String message) {
        int indentSize = 0;
        while (indentSize < message.length() && Character.isWhitespace(message.charAt(indentSize))) {
            indentSize++;
        }
        return new String[] {
            message.substring(0, indentSize), message.substring(indentSize)
        };
    }
}
