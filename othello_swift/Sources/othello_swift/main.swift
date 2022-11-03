#!/usr/bin/env swift

import ColorizeSwift

print("OTHELLO GAME - SWIFT".bold().green())
// try to read board size from command line args
var board_size: Int = CommandLine.argc >= 2 ? (Int(CommandLine.arguments[1]) ?? 0) : 0
if board_size > 0 {
    print("Using board size: \(board_size)")
} else {
    board_size = Othello.get_board_size()
}

let game = Othello(size: board_size)
game.play()
