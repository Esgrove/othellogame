package othello

enum class AnsiColor(private val code: String) {
    CYAN("\u001B[36m"),
    GREEN("\u001B[32m"),
    MAGENTA("\u001B[35m"),
    RED("\u001B[31m"),
    WHITE("\u001B[37m"),
    YELLOW("\u001B[33m"),
    BOLD("\u001B[1m"),
    RESET("\u001B[0m"),
    ;

    override fun toString(): String = code
}

/** Format string with colour.*/
fun getColor(text: String, color: AnsiColor, bold: Boolean = false): String {
    // Check if the text contains a reset code already
    val coloredText = text.replace(AnsiColor.RESET.toString(), "${AnsiColor.RESET}$color")

    if (bold) {
        return "$color${AnsiColor.BOLD}$coloredText${AnsiColor.RESET}"
    }

    return "$color$coloredText${AnsiColor.RESET}"
}

/** Print text with the specified colour.*/
fun printColor(text: String, color: AnsiColor) {
    println(getColor(text, color))
}

/** Print bold text with optional colour.*/
fun printBold(text: String, color: AnsiColor = AnsiColor.WHITE) {
    println(getColor(text, color, bold = true))
}

/** Print an error message with red colour.*/
fun printError(message: String) {
    val (indent, text) = splitLeadingWhitespace(message)
    println("$indent${getColor("Error: $text", AnsiColor.RED)}")
}

/** Print a warning message with yellow colour.*/
fun printWarn(message: String) {
    val (indent, text) = splitLeadingWhitespace(message)
    println("$indent${getColor("Warning: $text", AnsiColor.YELLOW)}")
}

/** Split a string into the leading whitespace and the rest of the string.*/
fun splitLeadingWhitespace(message: String): Pair<String, String> {
    val indentSize = message.takeWhile { it.isWhitespace() }.length
    return message.take(indentSize) to message.substring(indentSize)
}
