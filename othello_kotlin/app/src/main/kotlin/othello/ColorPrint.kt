package othello

import java.awt.Color

/** Format string with colour.*/
fun getColor(text: String, color: Color): String {
    val resetColor = "\u001B[0m"
    val ansiColor = when (color) {
        Color.CYAN -> "\u001B[36m"
        Color.GREEN -> "\u001B[32m"
        Color.MAGENTA -> "\u001B[35m"
        Color.RED -> "\u001B[31m"
        Color.WHITE -> "\u001B[37m"
        Color.YELLOW -> "\u001B[33m"
        else -> resetColor
    }

    // Check if the text contains a reset code already
    val coloredText = if (text.contains(resetColor)) {
        text.replace(resetColor, "$resetColor$ansiColor")
    } else {
        text
    }
    return "$ansiColor$coloredText$resetColor"
}

/** Print text with specified colour.*/
fun printColor(text: String, color: Color) {
    println(getColor(text, color))
}

/** Print bold text.*/
fun printBold(text: String) {
    println("\u001B[1m${text}\u001B[0m")
}

/** Print error message with red colour.*/
fun printError(message: String) {
    val (indent, text) = splitLeadingWhitespace(message)
    println("$indent${getColor("Error: $text", Color.RED)}")
}

/** Print warning message with yellow colour.*/
fun printWarn(message: String) {
    val (indent, text) = splitLeadingWhitespace(message)
    println("$indent${getColor("Warning: $text", Color.YELLOW)}")
}

/** Split a string into the leading whitespace and the rest of the string.*/
fun splitLeadingWhitespace(message: String): Pair<String, String> {
    val indentSize = message.takeWhile { it.isWhitespace() }.length
    return message.substring(0, indentSize) to message.substring(indentSize)
}
