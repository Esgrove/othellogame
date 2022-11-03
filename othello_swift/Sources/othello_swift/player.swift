
import Foundation

class Player {
    var can_play_: Bool = true
    var disk: Disk
    var human: Bool = true
    var rounds_played: Int = 0
    var show_helpers: Bool = true

    init(_ disk: Disk) {
        self.disk = disk
    }

    func can_play() -> Bool {
        can_play_
    }

    /// Play one round as this player.
    func play_one_move(board: inout Board) {
        print("Turn: \(disk)")
        let moves = board.possible_moves(disk: disk)
        if !moves.isEmpty {
            can_play_ = true
            if human && show_helpers {
                board.print_possible_moves(moves)
            }
            let chosen_move = human ? get_human_move(moves) : get_computer_move(moves)
            board.place_disk(move: chosen_move)
            board.print_score()
            rounds_played += 1
        } else {
            can_play_ = false
            print("  No moves available...".yellow())
        }
    }

    /// Set player to be controlled by human or computer.
    func set_human(_ is_human: Bool) {
        human = is_human
    }

    /// Return move chosen by computer.
    func get_computer_move(_ moves: [Move]) -> Move {
        print("  Computer plays...")
        // wait a bit and pick a random move
        let seconds = Double.random(in: 1 ... 2)
        Thread.sleep(forTimeInterval: seconds)
        // safe to force unwrap since moves will always contain elements
        let move = moves.randomElement()!
        print("  -> \(move.square)")
        return move
    }

    /// Return move chosen by a human player.
    func get_human_move(_ moves: [Move]) -> Move {
        while true {
            let square = get_square()
            // check if given square is one of the possible moves
            if let move = moves.first(where: { $0.square == square }) {
                return move
            }
            print_error("Can't place a \(disk) disk in square \(square)!", indent: 2)
        }
    }

    /// Ask human player for square coordinates.
    func get_square() -> Square {
        while true {
            print("  Give disk position (x,y): ", terminator: "")
            if let pos = readLine() {
                let coordinates = pos.components(separatedBy: ",").compactMap { Int($0) }
                if coordinates.count == 2 {
                    return Square(coordinates[0], coordinates[1])
                }
            }
            print_error("Give coordinates in the form 'x,y'!", indent: 2)
        }
    }

    func type_string() -> String {
        human ? "Human   " : "Computer"
    }
}

extension Player: CustomStringConvertible {
    var description: String {
        "\(disk) | \(type_string()) | Moves: \(rounds_played)"
    }
}
