package othello;

public class ColorPrint {
    /**
     * Format string with color.
     */
    public static String getColor(String text, AnsiColor color, boolean bold) {
        // Check if the text contains a reset code already
        String coloredText = text.replace(AnsiColor.RESET.toString(), AnsiColor.RESET + color.toString());

        if (bold) {
            return color + AnsiColor.BOLD.toString() + coloredText + AnsiColor.RESET;
        }

        return color + coloredText + AnsiColor.RESET;
    }

    /**
     * Print text with specified color.
     */
    public static void printColor(String text, AnsiColor color) {
        System.out.println(getColor(text, color, false));
    }

    /**
     * Print bold text.
     */
    public static void printBold(String text) {
        printBold(text, AnsiColor.WHITE);
    }

    /**
     * Print bold text with optional color.
     */
    public static void printBold(String text, AnsiColor color) {
        System.out.println(getColor(text, color, true));
    }

    /**
     * Print error message with red color.
     */
    public static void printError(String message) {
        String[] split = splitLeadingWhitespace(message);
        System.out.println(split[0] + getColor("Error: " + split[1], AnsiColor.RED, false));
    }

    /**
     * Print warning message with yellow color.
     */
    public static void printWarn(String message) {
        String[] split = splitLeadingWhitespace(message);
        System.out.println(split[0] + getColor("Warning: " + split[1], AnsiColor.YELLOW, false));
    }

    /**
     * Split a string into the leading whitespace and the rest of the string.
     */
    private static String[] splitLeadingWhitespace(String message) {
        int indentSize = 0;
        while (indentSize < message.length() && Character.isWhitespace(message.charAt(indentSize))) {
            indentSize++;
        }
        return new String[]{message.substring(0, indentSize), message.substring(indentSize)};
    }
}
