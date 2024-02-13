//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2024
//==========================================================

/// Handles game board state and logic.
class Board {
    let size: Int
    var board: [Disk]
    var emptySquares = Set<Square>()
    let stepDirections = [[0, -1], [0, 1], [1, 0], [-1, 0], [1, -1], [1, 1], [-1, 1], [-1, -1]]

    init(size: Int) {
        self.size = size
        // init game board with empty disks
        self.board = [Disk](repeating: Disk.empty, count: size * size)

        // set starting positions
        let pos = (self.size - 1) / 2
        let row: Int = self.size % 2 == 0 ? pos : pos - 1
        let col: Int = self.size / 2
        self.board[row * self.size + row] = Disk.white
        self.board[row * self.size + col] = Disk.black
        self.board[col * self.size + row] = Disk.black
        self.board[col * self.size + col] = Disk.white

        // keep track of empty squares on board to avoid checking already filled positions
        for y in 0 ..< self.size {
            for x in 0 ..< self.size {
                if self.board[y * self.size + x] == Disk.empty {
                    self.emptySquares.insert(Square(x, y))
                }
            }
        }
    }

    /// Return true if board contains empty squares.
    func canPlay() -> Bool {
        !self.emptySquares.isEmpty
    }

    /// Update board for given disk placement.
    func placeDisk(move: Move) {
        let start = move.square
        self.setSquare(start, move.disk)
        self.emptySquares.remove(start)
        for step in move.directions {
            var pos = start + step
            while self.getSquare(pos) == move.disk.opponent() {
                self.setSquare(pos, move.disk)
                pos += step
            }
        }
    }

    /// Returns a list of possible moves for given player.
    func possibleMoves(disk: Disk) -> [Move] {
        var moves = [Move]()
        let other: Disk = disk.opponent()
        for square in self.emptySquares {
            var value = 0
            var directions = [Square]()
            for dir in self.stepDirections {
                let step = Square(dir[0], dir[1])
                var pos = square + step
                // next square in this directions needs to be opponents disk
                if self.getSquare(pos) != other {
                    continue
                }
                var steps = 0
                // keep stepping forward while opponents disks are found
                while self.getSquare(pos) == other {
                    steps += 1
                    pos += step
                }
                // valid move if a line of opponents disks ends in own disk
                if self.getSquare(pos) != disk {
                    continue
                }
                value += steps
                directions.append(step)
            }
            if value > 0 {
                moves.append(Move(square, value, disk, directions))
            }
        }
        return moves.sorted()
    }

    /// Print available move coordinates and resulting points gained.
    func printPossibleMoves(_ moves: [Move]) {
        print("  Possible moves (\(moves.count)):".yellow())
        // convert board from Disk enums to strings
        var formattedBoard = self.board.map { $0.boardChar() }
        for move in moves {
            print("  \(move)")
            formattedBoard[move.square.y * self.size + move.square.x] = "\(move.value)".yellow()
        }
        // print board with move positions
        print("   ", terminator: "")
        for x in 0 ..< self.size {
            print(" \(x)".dim(), terminator: "")
        }
        for y in 0 ..< self.size {
            print("\n  \(y)".dim(), terminator: "")
            for x in 0 ..< self.size {
                print(" \(formattedBoard[y * self.size + x])", terminator: "")
            }
        }
        print("")
    }

    /// Returns the winner color.
    func result() -> Disk {
        let sum: Int = self.score()
        switch sum {
            case ..<0:
                return Disk.black
            case 1...:
                return Disk.white
            default:
                return Disk.empty
        }
    }

    /// Print current score for both players.
    func printScore() {
        let score = self.playerScores()
        print("\n\(self)")
        print(
            "Score: \(getColor(score.black, Disk.black.color())) |",
            "\(getColor(score.white, Disk.white.color()))"
        )
    }

    /// Get board status string for game log.
    func toLogEntry() -> String {
        let logChars = self.board.map { $0.boardChar(color: false) }
        return logChars.joined()
    }

    /// Check that the given coordinates are inside the board.
    private func checkCoordinates(_ x: Int, _ y: Int) -> Bool {
        x >= 0 && x < self.size && y >= 0 && y < self.size
    }

    /// Count and return the number of black and white disks.
    private func playerScores() -> (black: Int, white: Int) {
        let counts = self.board.reduce(into: (black: 0, white: 0)) { result, disk in
            switch disk {
                case .white:
                    result.white += 1
                case .black:
                    result.black += 1
                case .empty:
                    break
            }
        }
        return counts
    }

    /// Returns the total score.
    /// Positive value means more white disks and negative means more black disks.
    private func score() -> Int {
        self.board.reduce(0) { $0 + $1.rawValue }
    }

    /// Returns the state of the board (empty, white, black) at the given square.
    private func getSquare(_ square: Square) -> Disk? {
        self.checkCoordinates(square.x, square.y) ? self
            .board[square.y * self.size + square.x] : nil
    }

    /// Sets the given square to given value.
    private func setSquare(_ square: Square, _ disk: Disk) {
        if !self.checkCoordinates(square.x, square.y) {
            print("Invalid coordinates \(square)!")
        }
        self.board[square.y * self.size + square.x] = disk
    }
}

extension Board: CustomStringConvertible {
    var description: String {
        var text = " "
        for x in 0 ..< self.size {
            text += " \(x)".dim()
        }
        for y in 0 ..< self.size {
            text += "\n\(y)".dim()
            for x in 0 ..< self.size {
                let disk = self.board[y * self.size + x]
                text += " \(disk.boardChar())"
            }
        }
        return text
    }
}
