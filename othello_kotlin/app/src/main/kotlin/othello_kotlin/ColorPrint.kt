package othello_kotlin

import java.awt.Color

/** */
fun getColor(text: String, color: Color): String {
    val ansiColor = when (color) {
        Color.CYAN -> "\u001B[36m"
        Color.GREEN -> "\u001B[32m"
        Color.MAGENTA -> "\u001B[35m"
        Color.RED -> "\u001B[31m"
        Color.WHITE -> "\u001B[37m"
        Color.YELLOW -> "\u001B[33m"
        // Default to white
        else -> "\u001B[37m"
    }
    // \u001B[0m resets the colour
    return "$ansiColor$text\u001B[0m"
}

/** */
fun printColor(text: String, color: Color) {
    println(getColor(text, color))
}

/** */
fun printBold(text: String, indent: Int = 0) {
    val whitespace = if (indent > 0) " ".repeat(indent) else ""
    println("$whitespace\u001B[1m${text}\u001B[0m")
}

/** */
fun printError(message: String, indent: Int = 0) {
    val whitespace = if (indent > 0) " ".repeat(indent) else ""
    println("$whitespace${getColor("Error: $message", Color.RED)}")
}

/** */
fun printWarn(message: String, indent: Int = 0) {
    val whitespace = if (indent > 0) " ".repeat(indent) else ""
    println("$whitespace${getColor("Warning: $message", Color.YELLOW)}")
}
