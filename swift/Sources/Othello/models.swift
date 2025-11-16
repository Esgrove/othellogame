//==========================================================
// Models
// Basic types and methods
// Akseli Lukkarila
// 2019-2025
//==========================================================

import ColorizeSwift

/// Represents one game piece or lack of one.
enum Disk: Int, CustomStringConvertible {
    case black = -1
    case empty = 0
    case white = 1

    /// Returns a single character identifier string for the given disk.
    func boardChar(color: Bool = true) -> String {
        let character = (self == .empty) ? "_" : (self == .black ? "B" : "W")
        if color {
            return character.foregroundColor(self.color())
        } else {
            return character
        }
    }

    /// Return the associated color for this disk.
    func color() -> TerminalColor {
        switch self {
            case .black:
                TerminalColor.magenta1
            case .white:
                TerminalColor.cyan1
            case .empty:
                TerminalColor.white
        }
    }

    /// Returns the opposing disk.
    func opponent() -> Disk {
        switch self {
            case .black:
                .white
            case .white:
                .black
            case .empty:
                .empty
        }
    }

    var description: String {
        switch self {
            case .black:
                "BLACK".foregroundColor(self.color())
            case .white:
                "WHITE".foregroundColor(self.color())
            case .empty:
                "EMPTY"
        }
    }
}

/// Represents one square location on the board.
struct Square {
    var x: Int
    var y: Int

    init(_ x: Int, _ y: Int) {
        self.x = x
        self.y = y
    }

    init(fromSquare square: Square) {
        self.x = square.x
        self.y = square.y
    }
}

/// Represents a continuous line of squares in one direction.
///
/// The `step` field determines the direction on the board,
/// and `count` describes how many consecutive squares in that direction there are.
struct Direction {
    /// Direction of travel on the board
    var step: Square
    /// Number of consecutive same colour squares along this direction
    var count: Int

    init(_ step: Square, _ count: Int) {
        self.step = step
        self.count = count
    }

    func unpack() -> (Square, Int) {
        (self.step, self.count)
    }
}

/// Represents one possible disk placement for the given disk color.
struct Move {
    var square: Square
    var disk: Disk
    var value: Int
    var directions: [Direction]

    init(_ square: Square, _ value: Int, _ disk: Disk, _ directions: [Direction]) {
        self.square = square
        self.disk = disk
        self.value = value
        self.directions = directions
    }

    /// Format move for log entry
    func logEntry() -> String {
        "\(self.disk.boardChar(color: false)):\(self.square),\(self.value)"
    }

    /// Get all the squares playing this move will change.
    func affectedSquares() -> [Square] {
        var paths: [Square] = []
        for direction in self.directions {
            let (step, count) = direction.unpack()
            var pos: Square = self.square + step
            for _ in 0 ..< count {
                paths.append(pos)
                pos += step
            }
        }
        paths.sort()
        return paths
    }
}

extension Square: CustomStringConvertible {
    var description: String {
        "(\(self.x),\(self.y))"
    }
}

extension Square: ExpressibleByArrayLiteral {
    init(arrayLiteral: Int...) {
        assert(arrayLiteral.count == 2, "Square takes two int values!")
        self.x = arrayLiteral[0]
        self.y = arrayLiteral[1]
    }
}

extension Square {
    static func + (left: Square, right: Square) -> Square {
        [left.x + right.x, left.y + right.y]
    }

    static func += (left: inout Square, right: Square) {
        left = left + right
    }
}

extension Square: Comparable {
    static func == (left: Square, right: Square) -> Bool {
        left.x == right.x && left.y == right.y
    }

    static func < (left: Square, right: Square) -> Bool {
        if left.x < right.x { return true }
        if left.x == right.x && left.y < right.y { return true }
        return false
    }
}

extension Square: Hashable {
    func hash(into hasher: inout Hasher) {
        hasher.combine(self.x)
        hasher.combine(self.y)
    }
}

extension Move: CustomStringConvertible {
    var description: String {
        "Square: \(self.square) -> value: \(self.value)"
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
