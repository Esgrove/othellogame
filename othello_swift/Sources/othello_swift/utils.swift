//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2023
//==========================================================

import ColorizeSwift
import Foundation

import BuildInfo

let MIN_BOARD_SIZE: Int = 4
let MAX_BOARD_SIZE: Int = 10
let DEFAULT_BOARD_SIZE: Int = 8

enum Disk: Int, CustomStringConvertible {
    case black = -1
    case empty = 0
    case white = 1

    /// Returns a single character identifier string for the given disk.
    func boardChar() -> String {
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
    var directions: [Square]

    init(_ square: Square, _ value: Int, _ disk: Disk, _ directions: [Square]) {
        self.square = square
        self.disk = disk
        self.value = value
        self.directions = directions
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

func versionInfo() -> String {
    let versionNumber = VERSION
    let buildTime = BUILD_TIME
    let gitHash = GIT_HASH
    let gitBranch = GIT_BRANCH
    return "\(versionNumber) \(buildTime) \(gitBranch) \(gitHash)"
}
