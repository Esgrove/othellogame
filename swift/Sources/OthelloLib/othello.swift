//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

import Foundation

/// Gameplay loop and main logic.
public class Othello {
    private var board: Board
    private var settings: Settings
    private var playerBlack: Player
    private var playerWhite: Player
    private var gameLog: [String] = []
    private var gamesPlayed: Int = 0
    private var roundsPlayed: Int = 0

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
    private func initGame() {
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
            printBold("\nPlayers:")
            self.printStatus()
        }
    }

    /// Keep making moves until both players can't make a move any more.
    private func gameLoop() {
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

    /// Format game log with line numbers for each move.
    private func formatGameLog() -> String {
        self.gameLog
            .enumerated()
            .map { index, line in String(format: "%02d: %@", index + 1, line) }
            .joined(separator: "\n")
    }

    /// Print header for the current round.
    private func printRoundHeader() {
        if !self.settings.checkMode {
            printBold("\n=========== ROUND: \(self.roundsPlayed) ===========")
        }
    }

    /// Print footer after the game has ended.
    private func printGameEndFooter() {
        if !self.settings.checkMode {
            printBold("\n================================")
            printGreenBold("The game is finished!\n")
        }
    }

    /// Print game log which shows all moves made and the game board state after each move.
    private func printLog() {
        let formattedLog = self.formatGameLog()
        if !self.settings.checkMode {
            printYellowBold("Game log:")
            print(formattedLog)
        }
        let hexHash = calculateSha256(formattedLog)
        print(hexHash)
    }

    /// Print ending status and winner info.
    private func printResult() {
        printBold("Result:")
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
    private func printStatus() {
        print(self.playerBlack)
        print("\(self.playerWhite)\n")
        print(self.board)
    }

    /// Ask a question with two options, and return bool from user answer.
    private static func getAnswer(_ question: String, yes: String = "y", no: String = "n") -> Bool {
        print("\(question) (\(yes)/\(no))? ", terminator: "")
        guard let input = readLine() else {
            return false
        }
        return input.trimmingCharacters(in: .whitespaces).lowercased() == yes
    }

    /// Ask and return the desired board size.
    public static func getBoardSize() -> Int {
        print("Choose board size (default is \(DEFAULT_BOARD_SIZE)): ", terminator: "")
        guard let input = readLine(),
              let boardSize = Int(input.trimmingCharacters(in: .whitespaces))
        else {
            printWarn("Invalid size, defaulting to \(DEFAULT_BOARD_SIZE)...")
            return DEFAULT_BOARD_SIZE
        }
        if boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE {
            printYellow("Limiting board size to valid range \(MIN_BOARD_SIZE)..\(MAX_BOARD_SIZE)")
        }
        return boardSize.clamp(min: MIN_BOARD_SIZE, max: MAX_BOARD_SIZE)
    }
}
