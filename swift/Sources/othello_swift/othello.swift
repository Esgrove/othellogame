//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2024
//==========================================================

import ColorizeSwift

/// Gameplay loop and main logic.
class Othello {
    var board: Board
    var settings: Settings
    var playerBlack: Player
    var playerWhite: Player
    var roundsPlayed: Int = 0
    var gamesPlayed: Int = 0
    var gameLog: [String] = []

    init(_ settings: Settings) {
        self.board = Board(size: settings.boardSize)
        self.settings = settings
        self.playerBlack = Player.black(settings.toPlayerSettings())
        self.playerWhite = Player.white(settings.toPlayerSettings())
    }

    /// Play one full game of Othello.
    func play() {
        while true {
            self.initGame()
            self.gameLoop()
            self.printResult()
            if self.settings.showLog {
                self.printLog()
            }
            if self.settings.autoplayMode || !self.getAnswer("Would you like to play again") {
                break
            }
        }
    }

    /// Initialize game board and players for a new game.
    func initGame() {
        if self.gamesPlayed > 0 {
            self.board = Board(size: self.settings.boardSize)
            self.playerBlack.reset()
            self.playerWhite.reset()
            self.roundsPlayed = 0
        }

        if self.settings.autoplayMode {
            // Computer plays both
            self.playerWhite.setHuman(false)
            self.playerBlack.setHuman(false)
        } else if self.settings.useDefaults {
            // Default: play as black against white computer player
            self.playerWhite.setHuman(false)
        } else if self.getAnswer("Would you like to play against the computer") {
            if self.getAnswer("Would you like to play as black or white", yes: "b", no: "w") {
                self.playerWhite.setHuman(false)
            } else {
                self.playerBlack.setHuman(false)
            }
        }

        print("\nPlayers:".bold())
        self.printStatus()
    }

    /// Keep making moves until both players can't make a move any more.
    func gameLoop() {
        while self.board.canPlay() && (self.playerBlack.canPlay || self.playerWhite.canPlay) {
            self.roundsPlayed += 1
            printBold("\n=========== ROUND: \(self.roundsPlayed) ===========")
            for player in [self.playerBlack, self.playerWhite] {
                if let result = player.playOneMove(board: &self.board) {
                    self.gameLog.append("\(result);\(self.board.toLogEntry())")
                }
            }
            print("-------------------------------")
        }
    }

    /// Print game log which shows all moves made and the game board state after each move.
    func printLog() {
        var formattedLog = ""
        var index = 1
        for line in self.gameLog {
            formattedLog += String(format: "%02d: %@", index, line)
            if index < self.gameLog.count {
                formattedLog += "\n"
            }
            index += 1
        }

        let hexHash = calculateSHA256(formattedLog)

        printBold("Game log:", color: TerminalColor.yellow)
        print(formattedLog)
        print(hexHash)
    }

    /// Print ending status and winner info.
    func printResult() {
        print("\n===============================".bold())
        print("The game is finished!\n".green())
        print("Result:".bold())
        self.printStatus()
        print("")

        let winner = self.board.result()
        switch winner {
            case Disk.empty:
                print("The game ended in a tie...\n")
            default:
                print("The winner is \(winner)!\n")
        }
    }

    /// Print current board and player info.
    func printStatus() {
        print(self.playerBlack)
        print(self.playerWhite, terminator: "\n\n")
        print(self.board)
    }

    /// Ask a question with two options, and return bool from user answer.
    func getAnswer(_ question: String, yes: String = "y", no: String = "n") -> Bool {
        print("\(question) (\(yes)/\(no))? ", terminator: "")
        let ans = readLine()
        return ans?.lowercased() == yes.lowercased()
    }

    /// Ask and return the desired board size.
    class func getBoardSize() -> Int {
        while true {
            print("Choose board size (default is \(DEFAULT_BOARD_SIZE)): ", terminator: "")
            if let input = readLine() {
                // TODO: create clamp method
                if let size = Int(input) {
                    return max(MIN_BOARD_SIZE, min(size, MAX_BOARD_SIZE))
                }
            }
            printError("give a valid number...")
        }
    }
}
