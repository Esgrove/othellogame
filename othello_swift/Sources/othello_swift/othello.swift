//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

/// Gameplay loop and main logic.
class Othello {
    var boardSize: Int = 0
    var board: Board
    var playerBlack: Player = .init(Disk.black)
    var playerWhite: Player = .init(Disk.white)
    var roundsPlayed: Int = 0
    var gamesPlayed: Int = 0

    init(size: Int) {
        self.boardSize = size
        self.board = Board(size: self.boardSize)
    }

    /// Play one full game of Othello.
    func play() {
        while true {
            self.initGame()
            self.gameLoop()
            self.printResult()
            if !self.getAnswer("\nWould you like to play again") {
                break
            }
        }
    }

    /// Initialize game board and players for a new game.
    func initGame() {
        if self.gamesPlayed > 0 {
            self.board = Board(size: self.boardSize)
            self.playerBlack.reset()
            self.playerWhite.reset()
            self.roundsPlayed = 0
        }

        if self.getAnswer("Would you like to play against the computer") {
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
            print("\n=========== ROUND: \(self.roundsPlayed) ===========".bold())
            self.playerBlack.playOneMove(board: &self.board)
            print("-------------------------------")
            self.playerWhite.playOneMove(board: &self.board)
        }
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
                print("The game ended in a tie...")
            default:
                print("The winner is \(winner)!")
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
