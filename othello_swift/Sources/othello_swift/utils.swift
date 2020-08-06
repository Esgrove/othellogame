import ColorizeSwift

enum Disk: Int, CustomStringConvertible {
    case black = -1
    case white = 1
    case empty = 0
    
    /// Returns a single character identifier string for the given disk.
    func board_char() -> String {
        switch self {
            case .black:
                return "B".foregroundColor(self.color())
            case .white:
                return "W".foregroundColor(self.color())
            case .empty:
                return "_"
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
    func other_disk() -> Disk {
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

extension Square: Hashable {
    static func == (left: Square, right: Square) -> Bool {
        left.x == right.x && left.y == right.y
    }
    
    func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
    }
}

/// Represents one possible disk placement for given disk color.
struct Move {
    var square: Square
    var disk: Disk
    var value: Int
    var directions: [Square]
    
    init(_ square: Square, _ value: Int, _ disk: Disk, _ directions: [Square]) {
        self.square = square
        self.disk = disk
        self.value = value
        self.directions = directions
    }
}

extension Move: CustomStringConvertible {
    var description: String {
        "Square: \(self.square) -> value: \(self.value)"
    }
}

extension Move: Equatable {
    static func == (left: Move, right: Move) -> Bool {
        left.square == right.square && left.value == right.value && left.disk == right.disk
    }
}
