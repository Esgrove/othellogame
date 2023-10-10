#!/usr/bin/env swift

//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

import ColorizeSwift
import Foundation

// `main.swift`is automatically used as the entry point,
// without needing to call a main() function explicitly.

print("OTHELLO GAME - SWIFT".bold().green())

// Handle 'help' and 'version' arguments
for arg in CommandLine.arguments {
    switch arg {
        case "--version", "-v":
            // TODO: add version info
            print("Othello Swift \(version_info())")
            exit(0)
        case "--help", "-h":
            print("Othello Swift \(version_info())\n")
            print("USAGE: othello_swift [board size]")
            print("Optional arguments:")
            print("    -h | --help          Print usage and exit")
            print("    -v | --version       Print version info and exit")
            exit(1)
        default:
            break
    }
}

// Try to read board size from command line args
var board_size: Int = CommandLine.argc >= 2 ? (Int(CommandLine.arguments[1]) ?? 0) : 0
if board_size > 0 {
    if board_size < MIN_BOARD_SIZE || board_size > MAX_BOARD_SIZE {
        print("Error: unsupported board size: \(board_size)")
        exit(1)
    }
    print("Using board size: \(board_size)")
} else {
    // Otherwise ask user for board size
    board_size = Othello.get_board_size()
}

let game = Othello(size: board_size)
game.play()
