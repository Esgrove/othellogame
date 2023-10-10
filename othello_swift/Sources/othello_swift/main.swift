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
            print("Othello Swift \(versionInfo())")
            exit(0)
        case "--help", "-h":
            print("Othello Swift \(versionInfo())\n")
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
var boardSize: Int = CommandLine.argc >= 2 ? (Int(CommandLine.arguments[1]) ?? 0) : 0
if boardSize > 0 {
    if boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE {
        print("Error: unsupported board size: \(boardSize)")
        exit(1)
    }
    print("Using board size: \(boardSize)")
} else {
    // Otherwise ask user for board size
    boardSize = Othello.getBoardSize()
}

let game = Othello(size: boardSize)
game.play()
