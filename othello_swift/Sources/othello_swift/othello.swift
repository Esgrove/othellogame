//==========================================================
// Class Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

class Othello {
    var board_size: Int = 0
    var board: Board
    var player_black: Player = .init(Disk.black)
    var player_white: Player = .init(Disk.white)
    var rounds_played: Int = 0
    var games_played: Int = 0

    init(size: Int) {
        board_size = size
        board = Board(size: board_size)
    }

    /// Play one full game of Othello.
    func play() {
        while true {
            init_game()
            game_loop()
            print_result()
            if !get_answer("\nWould you like to play again") {
                break
            }
        }
    }

    /// Initialize game board and players for a new game.
    func init_game() {
        if (self.games_played > 0) {
            board = Board(size: board_size)
            player_black = Player(Disk.black)
            player_white = Player(Disk.white)
            rounds_played = 0
        }

        if get_answer("Would you like to play against the computer") {
            if get_answer("Would you like to play as black or white", yes: "b", no: "w") {
                player_white.set_human(false)
            } else {
                player_black.set_human(false)
            }
        }
        print("\nPlayers:".bold())
        print_status()
    }


    /// Keep making moves until both players can't make a move anymore.
    func game_loop() {
        while board.can_play() && (player_black.can_play() || player_white.can_play()) {
            rounds_played += 1
            print("\n=========== ROUND: \(rounds_played) ===========".bold())
            player_black.play_one_move(board: &board)
            print("-------------------------------")
            player_white.play_one_move(board: &board)
        }
    }

    /// Print ending status and winner info.
    func print_result() {
        print("\n===============================".bold())
        print("The game is finished!\n".green())
        print("Result:".bold())
        print_status()
        print("")

        let winner = board.result()
        switch winner {
            case Disk.empty:
                print("The game ended in a tie...")
            default:
                print("The winner is \(winner)!")
        }
    }

    /// Print current board and player info.
    func print_status() {
        print(player_black)
        print(player_white, terminator: "\n\n")
        print(board)
    }

    /// Ask a question with two options, and return bool from user answer.
    func get_answer(_ question: String, yes: String = "y", no: String = "n") -> Bool {
        print("\(question) (\(yes)/\(no))? ", terminator: "")
        let ans = readLine()
        return ans?.lowercased() == yes.lowercased()
    }

    /// Ask and return the desired board size.
    class func get_board_size() -> Int {
        while true {
            print("Choose board size (default is 8): ", terminator: "")
            if let input = readLine() {
                if let size = Int(input) {
                    return max(4, min(size, 16))
                }
            }
            print_error("give a valid number...")
        }
    }
}
