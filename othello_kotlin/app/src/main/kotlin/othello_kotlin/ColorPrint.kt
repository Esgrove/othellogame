package othello_kotlin

import java.awt.Color

fun colorPrint(text: String, color: Color): String {
    val ansiColor = when (color) {
        Color.WHITE -> "\u001B[37m"
        Color.CYAN -> "\u001B[36m"
        Color.MAGENTA -> "\u001B[35m"
        Color.RED -> "\u001B[31m"
        else -> "\u001B[37m" // Default to white
    }
    return "$ansiColor$text\u001B[0m"  // \u001B[0m resets the color
}

fun write(text: String, color: Color) {
    print(colorPrint(text, color))
}

fun writeLine(text: String, color: Color) {
    println(colorPrint(text, color))
}

fun error(message: String, indent: Int = 0) {
    val whitespace = if (indent > 0) " ".repeat(indent) else ""
    println("$whitespace${colorPrint("Error:", Color.RED)} $message")
}
