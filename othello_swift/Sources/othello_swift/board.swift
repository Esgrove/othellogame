
class Board {
    let size: Int
    var board: [Disk]
    var empty_squares = Set<Square>()
    let step_directions = [[0, -1], [0, 1], [1, 0], [-1, 0], [1, -1], [1, 1], [-1, 1], [-1, -1]]
    
    init(size: Int) {
        self.size = size
        // init game board with empty disks
        self.board = Array<Disk>(repeating: Disk.empty, count: size * size)

        // set starting positions
        let pos = (self.size - 1) / 2
        let row: Int = self.size % 2 == 0 ? pos : pos - 1
        let col: Int = self.size / 2
        self.board[row * self.size + row] = Disk.black
        self.board[row * self.size + col] = Disk.white
        self.board[col * self.size + row] = Disk.white
        self.board[col * self.size + col] = Disk.black

        // keep track of empty squares on board to avoid checking already filled positions
        for y in 0..<self.size {
            for x in 0..<self.size {
                if (board[y * self.size + x] == Disk.empty) {
                    self.empty_squares.insert(Square(x, y))
                }
            }
        }
    }
    
    /// Return true if board contains empty squares.
    func can_play() -> Bool {
        !self.empty_squares.isEmpty
    }

    /// Update board for given disk placement.
    func place_disk(move: Move) -> Void {
        let start = move.square
        self.set_square(start, move.disk)
        self.empty_squares.remove(start)
        for step in move.directions {
            var pos = start + step
            while self.get_square(pos) == move.disk.other_disk() {
                self.set_square(pos, move.disk)
                pos += step
            }
        }
    }
  
    /// Returns a list of possible moves for given player.
    func possible_moves(disk: Disk) -> [Move] {
        var moves = [Move]()
        let other: Disk = disk.other_disk()
        for square in self.empty_squares {
            var value = 0
            var directions = [Square]()
            for dir in self.step_directions {
                let step = Square(dir[0], dir[1])
                var pos = square + step
                // next square in this directions needs to be opponents disk
                if self.get_square(pos) != other {
                    continue
                }
                var steps = 0
                // keep stepping forward while opponents disks are found
                while self.get_square(pos) == other {
                    steps += 1
                    pos += step
                }
                // valid move if a line of opponents disks ends in own disk
                if self.get_square(pos) != disk {
                    continue
                }
                value += steps
                directions.append(step)
            }
            if value > 0 {
                moves.append(Move(square, value, disk, directions))
            }
        }
        return moves.sorted(by: { $0.value > $1.value })
    }
    
    func print_possible_moves(_ moves: [Move]) -> Void {
        print("  Possible plays (\(moves.count)):".yellow())
        for move in moves {
            print("  \(move)")
        }
    }
    
    /// Calculates the final score and returns the winner color.
    func result() -> Disk {
        let sum: Int = score()
        if (sum == 0) {
            return Disk.empty
        }
        return sum > 0 ? Disk.white : Disk.black
    }
    
    /// Print current score for both players.
    func print_score() -> Void {
        let score = self.player_scores()
        print("\n\(self)")
        // TODO: add colors
        print("Score: \(score.black) | \(score.white)")
    }
    /// Check that the given coordinates are inside the board.
    private func check_coordinates(_ x: Int, _ y: Int) -> Bool {
        0 <= x && x < self.size && 0 <= y && y < self.size
    }

    /// Count and return the number of black and white disks.
    private func player_scores() -> (black: Int, white: Int) {
        var black = 0
        var white = 0
        for disk in self.board {
            switch (disk) {
            case Disk.white:
                white += 1
            case Disk.black:
                black += 1
            case .empty:
                break
            }
        }
        return (black, white)
    }
    
    /// Returns the total score (positive means more white disks and negative means more black disks).
    private func score() -> Int {
        self.board.reduce(0) { $0 + $1.rawValue }
    }
    
    /// Returns the state of the board (empty, white, black) at the given square.
    private func get_square(_ square: Square) -> Disk? {
        self.check_coordinates(square.x, square.y) ? self.board[square.y * self.size + square.x] : nil
    }
    
    /// Sets the given square to given value.
    private func set_square(_ square: Square, _ disk: Disk) -> Void {
        if (!self.check_coordinates(square.x, square.y)) {
            print("Invalid coordinates \(square)!")
        }
        self.board[square.y * self.size + square.x] = disk
    }
}

extension Board: CustomStringConvertible {
    var description: String {
        var text = " "
        for x in 0..<self.size {
            text += " \(x)".dim()
        }
        for y in 0..<self.size {
            text += "\n\(y)".dim()
            for x in 0..<self.size {
                let disk = self.board[y * self.size + x]
                text += " \(disk.board_char())"
            }
        }
        return text
    }
}
