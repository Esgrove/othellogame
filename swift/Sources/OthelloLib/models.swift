//==========================================================
// Models
// Basic types and methods
// Akseli Lukkarila
// 2019-2026
//==========================================================

import ColorizeSwift

/// Represents one game piece or lack of one.
enum Disk: Int {
    case black = -1
    case empty = 0
    case white = 1

    /// Returns a single character identifier string for the given disk.
    func boardChar() -> String {
        switch self {
            case .black:
                "B"
            case .empty:
                "_"
            case .white:
                "W"
        }
    }

    /// Returns a coloured single character identifier string for the given disk.
    func boardCharWithColor() -> String {
        getColor(self.boardChar(), self.diskColor())
    }

    /// Return the associated colour for this disk.
    func diskColor() -> TerminalStyleCode {
        switch self {
            case .black:
                TerminalStyle.magenta
            case .empty:
                // Basic ANSI white (37)
                TerminalStyle.lightGray
            case .white:
                TerminalStyle.cyan
        }
    }

    /// Returns the disk formatted as a coloured string.
    func diskString() -> String {
        switch self {
            case .black:
                getColor("BLACK", self.diskColor())
            case .empty:
                getColor("EMPTY", self.diskColor())
            case .white:
                getColor("WHITE", self.diskColor())
        }
    }

    /// Return the opposing disk colour for this disk.
    func opponent() -> Disk {
        switch self {
            case .black:
                .white
            case .empty:
                .empty
            case .white:
                .black
        }
    }
}

/// Represents one step direction on the board.
struct Step {
    var x: Int
    var y: Int
}

/// Represents one square location on the board.
struct Square {
    var x: Int
    var y: Int

    /// Get the index of this square on the board.
    func boardIndex(boardSize: Int) -> Int {
        self.y * boardSize + self.x
    }
}

/// Represents a continuous line of squares in one direction.
///
/// The `step` field determines the direction on the board,
/// and `count` describes how many consecutive squares in that direction there are.
struct Direction {
    /// Direction of travel on the board
    var step: Step
    /// Number of consecutive same colour squares along this direction
    var count: Int
}

/// Represents one possible disk placement for the given disk colour.
struct Move {
    var square: Square
    var disk: Disk
    var value: Int
    var directions: [Direction]

    /// Format move for log entry
    func logEntry() -> String {
        "\(self.disk.boardChar()):\(self.square),\(self.value)"
    }

    /// Get all the squares playing this move will change.
    func affectedSquares() -> [Square] {
        var paths: [Square] = []
        for direction in self.directions {
            var pos: Square = self.square + direction.step
            for _ in 0 ..< direction.count {
                paths.append(pos)
                pos += direction.step
            }
        }
        paths.sort()
        return paths
    }
}

extension Disk: CustomStringConvertible {
    var description: String {
        self.diskString()
    }
}

extension Square: CustomStringConvertible {
    var description: String {
        "(\(self.x),\(self.y))"
    }
}

extension Move: CustomStringConvertible {
    var description: String {
        "Square: \(self.square) -> value: \(self.value)"
    }
}

extension Square {
    static func + (left: Square, right: Square) -> Square {
        Square(x: left.x + right.x, y: left.y + right.y)
    }

    static func + (left: Square, right: Step) -> Square {
        Square(x: left.x + right.x, y: left.y + right.y)
    }

    static func += (left: inout Square, right: Square) {
        left = left + right
    }

    static func += (left: inout Square, right: Step) {
        left = left + right
    }
}

extension Step {
    static func + (left: Step, right: Step) -> Step {
        Step(x: left.x + right.x, y: left.y + right.y)
    }

    static func += (left: inout Step, right: Step) {
        left = left + right
    }
}

extension Step: Hashable {}

extension Step: Comparable {
    static func < (left: Step, right: Step) -> Bool {
        left.x < right.x || (left.x == right.x && left.y < right.y)
    }
}

extension Square: Hashable {}

extension Square: Comparable {
    static func < (left: Square, right: Square) -> Bool {
        left.x < right.x || (left.x == right.x && left.y < right.y)
    }
}

extension Direction: Hashable {}

extension Direction: Comparable {
    static func < (left: Direction, right: Direction) -> Bool {
        left.step < right.step || (left.step == right.step && left.count < right.count)
    }
}

extension Move: Hashable {
    func hash(into hasher: inout Hasher) {
        hasher.combine(self.square)
        hasher.combine(self.value)
        hasher.combine(self.disk)
    }
}

extension Move: Comparable {
    static func == (left: Move, right: Move) -> Bool {
        left.square == right.square && left.value == right.value && left.disk == right.disk
    }

    static func < (left: Move, right: Move) -> Bool {
        left.value > right.value || (left.value == right.value && left.square < right.square)
    }
}
