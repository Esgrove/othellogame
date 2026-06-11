//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

import ColorizeSwift
import Foundation

/// Gameplay loop and main logic.
public class Othello {
    var board: Board
    var settings: Settings
    var playerBlack: Player
    var playerWhite: Player
    var gameLog: [String] = []
    var gamesPlayed: Int = 0
    var roundsPlayed: Int = 0

    /// Initialize Othello game.
    public init(_ settings: Settings) {
        self.board = Board(size: settings.boardSize)
        self.settings = settings
        self.playerBlack = Player.black(settings.toPlayerSettings())
        self.playerWhite = Player.white(settings.toPlayerSettings())
    }

    /// Play one full game of Othello.
    public func play() {
        while true {
            self.initGame()
            self.gameLoop()
            self.printResult()
            if self.settings.showLog {
                self.printLog()
            }
            if self.settings.autoplayMode || !Self.getAnswer("Would you like to play again") {
                break
            }
        }
    }

    /// Initialize game board and players for a new game.
    func initGame() {
        // Re-use existing objects instead of initializing new ones
        if self.gamesPlayed > 0 {
            self.board = Board(size: self.settings.boardSize)
            self.playerBlack.reset()
            self.playerWhite.reset()
            self.roundsPlayed = 0
            self.gameLog.removeAll()
        }
        if self.settings.autoplayMode {
            // Computer plays both
            self.playerBlack.setComputer()
            self.playerWhite.setComputer()
        } else if self.settings.useDefaults {
            // Default: play as black against white computer player
            self.playerWhite.setComputer()
        } else if Self.getAnswer("Would you like to play against the computer") {
            if Self.getAnswer("Would you like to play as black or white", yes: "b", no: "w") {
                self.playerWhite.setComputer()
            } else {
                self.playerBlack.setComputer()
            }
        }
        if !self.settings.checkMode {
            print("\nPlayers:".bold())
            self.printStatus()
        }
    }

    /// Keep making moves until both players can't make a move any more.
    func gameLoop() {
        while self.board.canPlay(), self.playerBlack.canPlay || self.playerWhite.canPlay {
            self.roundsPlayed += 1
            self.printRoundHeader()
            for player in [self.playerBlack, self.playerWhite] {
                if let result = player.playOneMove(board: self.board) {
                    self.gameLog.append("\(result);\(self.board.logEntry())")
                }
                if !self.settings.checkMode {
                    print("--------------------------------")
                }
            }
        }
        self.gamesPlayed += 1
        self.printGameEndFooter()
    }

    func formatGameLog() -> String {
        self.gameLog
            .enumerated()
            .map { index, line in String(format: "%02d: %@", index + 1, line) }
            .joined(separator: "\n")
    }

    func printRoundHeader() {
        if !self.settings.checkMode {
            print("\n=========== ROUND: \(self.roundsPlayed) ===========".bold())
        }
    }

    func printGameEndFooter() {
        if !self.settings.checkMode {
            print("\n================================".bold())
            print(getColor("The game is finished!\n", TerminalStyle.green, bold: true))
        }
    }

    /// Print game log which shows all moves made and the game board state after each move.
    func printLog() {
        let formattedLog = self.formatGameLog()
        if !self.settings.checkMode {
            print(getColor("Game log:", TerminalStyle.yellow, bold: true))
            print(formattedLog)
        }
        let hexHash = calculateSha256(formattedLog)
        print(hexHash)
    }

    /// Print ending status and winner info.
    func printResult() {
        print("Result:".bold())
        self.printStatus()
        print("")

        let winner = self.board.result()
        if winner == .empty {
            print("The game ended in a tie...\n")
        } else {
            print("The winner is \(winner)!\n")
        }
    }

    /// Print current board and player info.
    func printStatus() {
        print(self.playerBlack)
        print("\(self.playerWhite)\n")
        print(self.board)
    }

    /// Ask a question with two options, and return bool from user answer.
    static func getAnswer(_ question: String, yes: String = "y", no: String = "n") -> Bool {
        print("\(question) (\(yes)/\(no))? ", terminator: "")
        guard let input = readLine() else {
            return false
        }
        return input.trimmingCharacters(in: .whitespaces).lowercased() == yes
    }

    /// Ask and return the desired board size.
    public static func getBoardSize() -> Int {
        print("Choose board size (default is \(DEFAULT_BOARD_SIZE)): ", terminator: "")
        if let input = readLine(),
           let boardSize = Int(input.trimmingCharacters(in: .whitespaces))
        {
            if boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE {
                print("Limiting board size to valid range \(MIN_BOARD_SIZE)..\(MAX_BOARD_SIZE)"
                    .yellow())
            }
            return boardSize.clamp(min: MIN_BOARD_SIZE, max: MAX_BOARD_SIZE)
        }
        printWarn("Invalid size, defaulting to \(DEFAULT_BOARD_SIZE)...")
        return DEFAULT_BOARD_SIZE
    }
}
