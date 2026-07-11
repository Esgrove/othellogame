package othello

/** Defines ANSI escape codes for adding colours to console output. */
enum class AnsiColor(internal val code: String) {
    CYAN("36"),
    GREEN("32"),
    MAGENTA("35"),
    RED("31"),
    WHITE("37"),
    YELLOW("33"),
    BOLD("1"),
    RESET("0"),
    ;

    override fun toString(): String = "\u001B[${code}m"
}

/** Format string with colour. */
fun getColor(text: String, color: AnsiColor, bold: Boolean = false): String {
    // Check if the text contains a reset code already
    val coloredText = text.replace(AnsiColor.RESET.toString(), "${AnsiColor.RESET}$color")

    if (bold) {
        return "\u001B[${AnsiColor.BOLD.code};${color.code}m$coloredText${AnsiColor.RESET}"
    }

    return "$color$coloredText${AnsiColor.RESET}"
}

/** Print text with specified colour. */
fun printColor(text: String, color: AnsiColor) {
    println(getColor(text, color))
}

/** Print bold text with optional colour. */
fun printBold(text: String, color: AnsiColor? = null) {
    if (color != null) {
        println(getColor(text, color, bold = true))
    } else {
        println(getColor(text, AnsiColor.BOLD))
    }
}

/** Print text in green. */
fun printGreen(text: String) {
    printColor(text, AnsiColor.GREEN)
}

/** Print bold text in green. */
fun printGreenBold(text: String) {
    printBold(text, AnsiColor.GREEN)
}

/** Print text in yellow. */
fun printYellow(text: String) {
    printColor(text, AnsiColor.YELLOW)
}

/** Print bold text in yellow. */
fun printYellowBold(text: String) {
    printBold(text, AnsiColor.YELLOW)
}

/** Print text in red. */
fun printRed(text: String) {
    printColor(text, AnsiColor.RED)
}

/** Print bold text in red. */
fun printRedBold(text: String) {
    printBold(text, AnsiColor.RED)
}

/** Print error message with red colour. */
fun printError(message: String) {
    val (indent, text) = splitLeadingWhitespace(message)
    println("$indent${getColor("Error: $text", AnsiColor.RED)}")
}

/** Print warning message with yellow colour. */
fun printWarn(message: String) {
    val (indent, text) = splitLeadingWhitespace(message)
    println("$indent${getColor("Warning: $text", AnsiColor.YELLOW)}")
}

/** Split a string into the leading whitespace and the rest of the string. */
fun splitLeadingWhitespace(message: String): Pair<String, String> {
    val indentSize = message.takeWhile { it.isWhitespace() }.length
    return message.take(indentSize) to message.substring(indentSize)
}
