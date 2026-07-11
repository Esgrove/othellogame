//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2026
//==========================================================

import ColorizeSwift

private let UP = 1
private let DOWN = -1
private let LEFT = -1
private let RIGHT = 1
private let STILL = 0

/// All possible step directions for a square on the board.
private let STEP_DIRECTIONS: [Step] = [
    Step(x: DOWN, y: LEFT),
    Step(x: DOWN, y: RIGHT),
    Step(x: DOWN, y: STILL),
    Step(x: STILL, y: LEFT),
    Step(x: STILL, y: RIGHT),
    Step(x: UP, y: LEFT),
    Step(x: UP, y: RIGHT),
    Step(x: UP, y: STILL),
]

/// Handles game board state and logic.
class Board {
    private var board: [Disk]
    private var emptySquares: Set<Square>
    private let indices: [Int]
    let size: Int

    /// Initialize a new board for the given board size.
    init(size: Int) {
        let board = Self.initBoard(size)
        // Index list (0...size) to avoid repeating same range in loops.
        let indices = Array(0 ..< size)

        // Keep track of empty squares on board to avoid checking already filled positions.
        let emptySquares = Self.initEmptySquares(size, board)

        self.board = board
        self.emptySquares = emptySquares
        self.indices = indices
        self.size = size
    }

    /// Return true if board contains empty squares.
    func canPlay() -> Bool {
        !self.emptySquares.isEmpty
    }

    /// Update board for given disk placement.
    func placeDisk(_ chosenMove: Move) {
        let start = chosenMove.square
        guard let disk = self.getSquare(start) else {
            preconditionFailure("Invalid coordinates: \(start)")
        }
        precondition(disk == .empty, "Trying to place disk to an occupied square: \(start)!")
        self.setSquare(start, chosenMove.disk)
        self.emptySquares.remove(start)
        for square in chosenMove.affectedSquares() {
            self.setSquare(square, chosenMove.disk)
        }
    }

    /// Returns a list of possible moves for the given player.
    func possibleMoves(disk: Disk) -> [Move] {
        var moves: [Move] = []
        let opposingDisk = disk.opponent()
        for square in self.emptySquares {
            var value = 0
            var directions: [Direction] = []
            for step in STEP_DIRECTIONS {
                var pos = square + step
                // Next square in this direction needs to be the opposing disk
                if self.getSquare(pos) != opposingDisk {
                    continue
                }
                var numSteps = 0
                // Keep stepping over opponents disks
                while self.getSquare(pos) == opposingDisk {
                    numSteps += 1
                    pos += step
                }
                // Valid move only if a line of opposing disks ends with own disk
                if self.getSquare(pos) == disk {
                    directions.append(Direction(step: step, count: numSteps))
                    value += numSteps
                }
            }
            if value > 0 {
                moves.append(Move(square: square, disk: disk, value: value, directions: directions))
            }
        }
        moves.sort()
        return moves
    }

    /// Print board with available move coordinates and the resulting points gained.
    func printPossibleMoves(_ moves: [Move]) {
        printYellow("  Possible moves (\(moves.count)):")
        // Convert board from Disk enums to strings
        var formattedBoard = self.board.map { $0.boardCharWithColor() }

        // Add possible moves to board
        for possibleMove in moves {
            let index = self.squareIndex(possibleMove.square)
            formattedBoard[index] = "\(possibleMove.value)".yellow()
            print("  \(possibleMove)")
        }
        // Print board with move positions
        let header = (["    "] + self.indices.map { "\($0)".bold() })
            .joined(separator: " ")

        let board = self.indices
            .map { y in
                let row = self.indices
                    .map { x in formattedBoard[y * self.size + x] }
                    .joined(separator: " ")
                return "  \("\(y)".bold()) \(row)"
            }
            .joined(separator: "\n")

        print("\(header)\n\(board)")
    }

    /// Print current score for both players.
    func printScore() {
        let (black, white) = self.playerScores()
        print("\n\(self)")
        print(
            "Score: \(getColor(black, TerminalStyle.magenta)) | \(getColor(white, TerminalStyle.cyan))"
        )
    }

    /// Returns the winning disk colour. Empty indicates a draw.
    func result() -> Disk {
        let totalScore = self.score()
        if totalScore > 0 {
            return .white
        }
        if totalScore < 0 {
            return .black
        }
        return .empty
    }

    /// Get board status string for game log.
    func logEntry() -> String {
        self.board.map { $0.boardChar() }.joined()
    }

    /// Check that the given coordinates are valid (inside the board).
    private func checkCoordinates(_ x: Int, _ y: Int) -> Bool {
        x >= 0 && x < self.size && y >= 0 && y < self.size
    }

    /// Check that the given square is valid (inside the board).
    private func checkSquare(_ square: Square) -> Bool {
        self.checkCoordinates(square.x, square.y)
    }

    /// Returns the state of the board (empty, white, black) at the given square.
    func getSquare(_ square: Square) -> Disk? {
        if self.checkSquare(square) {
            let index = self.squareIndex(square)
            return self.board[index]
        }
        return nil
    }

    /// Map square to board index.
    private func squareIndex(_ square: Square) -> Int {
        square.y * self.size + square.x
    }

    /// Count and return the number of black and white disks.
    func playerScores() -> (black: Int, white: Int) {
        var black = 0
        var white = 0
        for disk in self.board {
            switch disk {
                case .black:
                    black += 1
                case .white:
                    white += 1
                case .empty:
                    break
            }
        }
        return (black, white)
    }

    /// Returns the total score.
    /// Positive value means more white disks and negative means more black disks.
    func score() -> Int {
        self.board.reduce(0) { $0 + $1.rawValue }
    }

    /// Sets the given square to the given value.
    private func setSquare(_ square: Square, _ disk: Disk) {
        precondition(self.checkSquare(square), "Invalid coordinates: \(square)")
        let index = self.squareIndex(square)
        self.board[index] = disk
    }

    /// Initialize game board with starting disk positions.
    private static func initBoard(_ size: Int) -> [Disk] {
        // Initialize game board with empty disks
        var board = [Disk](repeating: .empty, count: size * size)
        // Set starting positions
        let row = size % 2 == 0 ? (size - 1) / 2 : (size - 1) / 2 - 1
        let col = size / 2
        board[row * size + row] = .white
        board[row * size + col] = .black
        board[col * size + row] = .black
        board[col * size + col] = .white
        return board
    }

    /// Initialize empty squares for the board.
    private static func initEmptySquares(_ size: Int, _ board: [Disk]) -> Set<Square> {
        var emptySquares = Set<Square>()
        for index in 0 ..< board.count where board[index] == .empty {
            emptySquares.insert(Square(x: index % size, y: index / size))
        }
        return emptySquares
    }
}

extension Board: CustomStringConvertible {
    var description: String {
        // Horizontal indices
        var text = "  " + self.indices.map { "\($0)" }.joined(separator: " ").bold()
        for y in self.indices {
            // Vertical index
            text += "\n\("\(y)".bold())"
            // Row values
            for x in self.indices {
                text += " \(self.board[y * self.size + x].boardCharWithColor())"
            }
        }
        return text
    }
}
