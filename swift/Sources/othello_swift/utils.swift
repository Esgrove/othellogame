//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2024
//==========================================================

import ColorizeSwift
import Crypto
import Foundation

import BuildInfo

let MIN_BOARD_SIZE: Int = 4
let MAX_BOARD_SIZE: Int = 10
let DEFAULT_BOARD_SIZE: Int = 8

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
    var directions: [Square]

    init(_ square: Square, _ value: Int, _ disk: Disk, _ directions: [Square]) {
        self.square = square
        self.disk = disk
        self.value = value
        self.directions = directions
    }

    func logEntry() -> String {
        return "\(self.disk.boardChar(color: false)):\(self.square),\(self.value)"
    }
}

/// Game settings
struct Settings {
    var boardSize: Int
    var autoplayMode: Bool
    var useDefaults: Bool
    var showHelpers: Bool
    var showLog: Bool
    var testMode: Bool

    // Get player setting values from overall game settings.
    func toPlayerSettings() -> PlayerSettings {
        return PlayerSettings(
            showHelpers: self.showHelpers,
            testMode: self.testMode
        )
    }
}

/// Player settings.
struct PlayerSettings {
    var showHelpers: Bool
    var testMode: Bool
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

/// Calculate SHA256 hash for the given string.
func calculateSHA256(_ text: String) -> String {
    let data = text.data(using: .utf8)!
    let hash = SHA256.hash(data: data)

    return hash.map { String(format: "%02x", $0) }.joined()
}

/// Return formatted build version information.
func versionInfo() -> String {
    let versionNumber = VERSION
    let buildTime = BUILD_TIME
    let gitHash = GIT_HASH
    let gitBranch = GIT_BRANCH
    return "\(versionNumber) \(buildTime) \(gitBranch) \(gitHash)"
}
