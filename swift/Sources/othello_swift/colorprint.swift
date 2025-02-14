//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2025
//==========================================================

import ColorizeSwift

/// Format string with colour.
func getColor<T>(_ message: T, _ color: TerminalColor, bold: Bool = false) -> String {
    bold ? "\(message)".foregroundColor(color).bold() : "\(message)".foregroundColor(color)
}

/// Print text with specified colour.
func printColor<T>(_ message: T, _ color: TerminalColor, bold: Bool = false) {
    print(getColor(message, color, bold: bold))
}

/// Print bold text with specified colour.
func printBold<T>(_ message: T, color: TerminalColor = TerminalColor.white) {
    print(getColor(message, color, bold: true))
}

/// Print error message with red colour.
func printError(_ message: String) {
    let (indent, text) = splitLeadingWhitespace(message)
    print("\(indent)Error: \(text)".red())
}

/// Print warning message with yellow colour.
func printWarn(_ message: String) {
    let (indent, text) = splitLeadingWhitespace(message)
    print("\(indent)Warning: \(text)".yellow())
}

/// Split a string into the leading whitespace and the rest of the string.
func splitLeadingWhitespace(_ message: String) -> (String, String) {
    let indentSize = message.prefix(while: { $0.isWhitespace }).count
    let index = message.index(message.startIndex, offsetBy: indentSize)
    return (String(message[..<index]), String(message[index...]))
}
