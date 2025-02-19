//==========================================================
// Types
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
                return TerminalColor.magenta1
            case .white:
                return TerminalColor.cyan1
            case .empty:
                return TerminalColor.white
        }
    }

    /// Returns the opposing disk.
    func opponent() -> Disk {
        switch self {
            case .black:
                return .white
            case .white:
                return .black
            case .empty:
                return .empty
        }
    }

    var description: String {
        switch self {
            case .black:
                return "BLACK".foregroundColor(self.color())
            case .white:
                return "WHITE".foregroundColor(self.color())
            case .empty:
                return "EMPTY"
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

/// Represents one possible disk placement for the given disk color.
struct Move {
    var square: Square
    var disk: Disk
    var value: Int
    var directions: [(square: Square, count: Int)]

    init(_ square: Square, _ value: Int, _ disk: Disk, _ directions: [(Square, Int)]) {
        self.square = square
        self.disk = disk
        self.value = value
        self.directions = directions
    }

    /// Format move for log entry
    func logEntry() -> String {
        return "\(self.disk.boardChar(color: false)):\(self.square),\(self.value)"
    }

    /// Get all the squares playing this move will change.
    func affectedSquares() -> [Square] {
        var paths: [Square] = []
        for (step, count) in self.directions {
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
