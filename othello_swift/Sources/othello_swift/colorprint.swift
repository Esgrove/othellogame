//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2023
//==========================================================

import ColorizeSwift

func get_color<T>(_ message: T, _ color: TerminalColor) -> String {
    "\(message)".foregroundColor(color)
}

func print_color<T>(_ message: T, _ color: TerminalColor) {
    print(get_color(message, color))
}

func print_error(_ message: String, indent: Int = 0) {
    let whitespace = String(repeating: " ", count: indent)
    print("\(whitespace)Error: ".red() + message)
}
