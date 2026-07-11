//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2026
//==========================================================

import ColorizeSwift
import Foundation

/// Format string with colour.
func getColor(_ message: some Any, _ color: TerminalStyleCode, bold: Bool = false) -> String {
    // Check if the text contains a reset code already
    let text = "\(message)".replacingOccurrences(
        of: TerminalStyle.reset.open,
        with: TerminalStyle.reset.open + color.open
    )
    if bold {
        // Compose a combined ANSI code, for example "\u{001B}[1;32m" for bold green
        let boldColor = color.open.replacingOccurrences(of: "[", with: "[1;")
        return boldColor + text + TerminalStyle.reset.open
    }
    return color.open + text + TerminalStyle.reset.open
}

/// Print bold text.
func printBold(_ text: String) {
    print(text.bold())
}

/// Print text in green.
func printGreen(_ text: String) {
    print(text.green())
}

/// Print bold text in green.
public func printGreenBold(_ text: String) {
    print(getColor(text, TerminalStyle.green, bold: true))
}

/// Print text in yellow.
func printYellow(_ text: String) {
    print(text.yellow())
}

/// Print bold text in yellow.
func printYellowBold(_ text: String) {
    print(getColor(text, TerminalStyle.yellow, bold: true))
}

/// Print text in red.
func printRed(_ text: String) {
    print(text.red())
}

/// Print bold text in red.
func printRedBold(_ text: String) {
    print(getColor(text, TerminalStyle.red, bold: true))
}

/// Print error message with red colour.
public func printError(_ message: String) {
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
