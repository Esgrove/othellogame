#!/usr/bin/env swift

//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2025
//==========================================================

import ArgumentParser
import ColorizeSwift
import Foundation

struct OthelloSwift: ParsableCommand {
    static var configuration = CommandConfiguration(
        abstract: "A simple Othello CLI game implementation in Swift",
        usage: "othello_swift [options] [size]"
    )

    @Argument(help: "Optional board size (\(MIN_BOARD_SIZE)..\(MAX_BOARD_SIZE))")
    var size: Int?

    @Flag(name: .shortAndLong, help: "Enable autoplay mode with computer control")
    var autoplay: Bool = false

    @Flag(name: .shortAndLong, help: "Autoplay and only print result")
    var check: Bool = false

    @Flag(name: [.short, .customLong("default")], help: "Play with default settings")
    var useDefaultSettings: Bool = false

    @Flag(name: .shortAndLong, help: "Show game log at the end")
    var log: Bool = false

    @Flag(name: .shortAndLong, help: "Hide disk placement hints")
    var noHelpers: Bool = false

    @Flag(name: .shortAndLong, help: "Enable test mode with deterministic computer moves")
    var test: Bool = false

    @Flag(name: .shortAndLong, help: "Print version and exit")
    var version: Bool = false

    mutating func run() throws {
        if self.version {
            print("Othello Swift \(versionInfo())")
            Self.exit(withError: ExitCode.success)
        }

        print("OTHELLO GAME - SWIFT".bold().green())

        // Try to read board size from command line args
        var boardSize: Int = self.size ?? 0
        if boardSize != 0 {
            if boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE {
                printError("Unsupported board size: \(boardSize)")
                Self.exit(withError: ExitCode.failure)
            }
            print("Using board size: \(boardSize)")
        } else if self.autoplay || self.useDefaultSettings {
            boardSize = DEFAULT_BOARD_SIZE
        } else {
            // Otherwise ask user for board size
            boardSize = Othello.getBoardSize()
        }

        let settings = Settings(
            boardSize: boardSize,
            autoplayMode: autoplay || self.check,
            checkMode: self.check,
            showHelpers: !self.noHelpers,
            showLog: self.log || self.check,
            testMode: self.test || self.check,
            useDefaults: self.useDefaultSettings
        )

        Othello(settings).play()
    }
}

OthelloSwift.main()
