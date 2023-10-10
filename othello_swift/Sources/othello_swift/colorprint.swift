//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2023
//==========================================================

import ColorizeSwift

func getColor<T>(_ message: T, _ color: TerminalColor) -> String {
    "\(message)".foregroundColor(color)
}

func printColor<T>(_ message: T, _ color: TerminalColor) {
    print(getColor(message, color))
}

func printError(_ message: String) {
    let (indent, text) = splitLeadingWhitespace(message)
    print("\(indent)Error: \(text)".red())
}

func printWarn(_ message: String) {
    let (indent, text) = splitLeadingWhitespace(message)
    print("\(indent)Warning: \(text)".yellow())
}

/// Split a string to the leading whitespace and rest of the string.
func splitLeadingWhitespace(message: String) -> (String, String) {
    let indentSize = message.prefix(while: { $0.isWhitespace }).count
    let index = message.index(message.startIndex, offsetBy: indentSize)
    return (String(message[..<index]), String(message[index...]))
}
