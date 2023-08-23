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

print("OTHELLO GAME - SWIFT".bold().green())

for argument in CommandLine.arguments {
    switch argument {
        case "--version", "-v":
            print("Othello Swift")
            exit(0)
        case "--help", "-h":
            print("Othello Swift")
            print("USAGE: othello_swift [board size]")
            exit(1)
        default:
            break
    }
}

// try to read board size from command line args
var board_size: Int = CommandLine.argc >= 2 ? (Int(CommandLine.arguments[1]) ?? 0) : 0
if board_size > 0 {
    if board_size < 4 || board_size > 10 {
        print("Error: unsupported board size: \(board_size)")
        exit(1)
    }
    print("Using board size: \(board_size)")
} else {
    board_size = Othello.get_board_size()
}

let game = Othello(size: board_size)
game.play()
