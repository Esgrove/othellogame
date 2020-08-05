
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
        self.can_play_
    }
    
    /// Play one round as this player.
    func play_one_move(board: inout Board) -> Void {
        print("Turn: \(self.disk)")
        let moves = board.possible_moves(disk: self.disk)
        if !moves.isEmpty {
            self.can_play_ = true
            if self.human && self.show_helpers {
                board.print_possible_moves(moves)
            }
            let chosen_move = self.human ? self.get_human_move(moves) : self.get_computer_move(moves)
            board.place_disk(move: chosen_move)
            board.print_score()
            self.rounds_played += 1
        } else {
            self.can_play_ = false
            print("  No moves available...")
        }
    }
    
    /// Set player to be controlled by human or computer.
    func set_human(_ is_human: Bool) -> Void {
        self.human = is_human
    }
    
    /// Return move chosen by computer.
    func get_computer_move(_ moves: [Move]) -> Move {
        print("  Computer plays...")
        // wait a bit and pick a random move
        let seconds = Double.random(in: 1...2)
        Thread.sleep(forTimeInterval: seconds)
        let move = moves.randomElement()!
        print("  -> \(move.square)")
        return move
    }
    
    /// Return move chosen by a human player.
    func get_human_move(_ moves: [Move]) -> Move {
        while true {
            let square = self.get_square()
            // check if given square is one of the possible moves
            if let move = moves.first(where: {$0.square == square} ) {
                return move
            }
            print("Can't place a \(self.disk) disk in square \(square)!")
        }
    }
    
    /// Ask human player for square coordinates.
    func get_square() -> Square {
        while true {
            print("  Give disk position (x,y): ", terminator: "")
            if let pos = readLine() {
                let coordinates = pos.components(separatedBy: ",").compactMap({ Int($0) })
                if coordinates.count == 2 {
                    return Square(coordinates[0], coordinates[1])
                }
            }
            print("Give coordinates in the form 'x,y'!")
        }
    }
        
    func type_string() -> String {
        self.human ? "Human   " : "Computer"
    }
}

extension Player: CustomStringConvertible {
    var description: String {
        "\(self.disk) | \(self.type_string()) | Moves: \(self.rounds_played)"
    }
}
