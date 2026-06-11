//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2026
//==========================================================

import ColorizeSwift
import Foundation

/// Defines one player that can be either human or computer controlled.
class Player {
    var canPlay: Bool = true
    var disk: Disk
    var playerType: PlayerType = .human
    var roundsPlayed: Int = 0
    var settings: PlayerSettings

    /// Initialize new player for the given disk color.
    init(disk: Disk, settings: PlayerSettings) {
        self.disk = disk
        self.settings = settings
    }

    /// Shorthand to initialize a new player for black disks.
    static func black(_ settings: PlayerSettings) -> Player {
        Player(disk: .black, settings: settings)
    }

    /// Shorthand to initialize a new player for white disks.
    static func white(_ settings: PlayerSettings) -> Player {
        Player(disk: .white, settings: settings)
    }

    /// Play one round as this player.
    func playOneMove(board: Board) -> String? {
        if !self.settings.checkMode {
            print("Turn: \(self.disk)")
        }
        let moves = board.possibleMoves(disk: self.disk)
        if moves.isEmpty {
            self.canPlay = false
            if !self.settings.checkMode {
                print("  No moves available...".yellow())
            }
            return nil
        }
        self.canPlay = true
        if self.human(), self.settings.showHelpers, !self.settings.checkMode {
            board.printPossibleMoves(moves)
        }
        let chosenMove = self.human() ? self.getHumanMove(moves) : self.getComputerMove(moves)
        board.placeDisk(chosenMove)
        if !self.settings.checkMode {
            board.printScore()
        }
        self.roundsPlayed += 1
        if !self.settings.testMode {
            Thread.sleep(forTimeInterval: 1)
        }
        return chosenMove.logEntry()
    }

    /// Reset player status for a new game.
    func reset() {
        self.canPlay = true
        self.roundsPlayed = 0
    }

    /// Returns true if player is controlled by a human player.
    func human() -> Bool {
        self.playerType.human()
    }

    /// Returns true if player is controlled by computer.
    func computer() -> Bool {
        self.playerType.computer()
    }

    /// Set the player as human or computer controlled.
    func setPlayerType(_ playerType: PlayerType) {
        self.playerType = playerType
    }

    /// Set the player as human controlled.
    func setHuman() {
        self.setPlayerType(.human)
    }

    /// Set the player as computer controlled.
    func setComputer() {
        self.setPlayerType(.computer)
    }

    /// Return move chosen by computer.
    func getComputerMove(_ moves: [Move]) -> Move {
        if !self.settings.checkMode {
            print("  Computer plays...")
        }
        let chosenMove: Move
        if self.settings.testMode {
            chosenMove = moves[0]
        } else {
            // Wait a bit and pick a random move
            Thread.sleep(forTimeInterval: Double.random(in: 1 ..< 2))
            chosenMove = moves.randomElement()!
        }
        if !self.settings.checkMode {
            print("  \(chosenMove.square) -> \(chosenMove.value)")
        }
        return chosenMove
    }

    /// Return move chosen by a human player.
    func getHumanMove(_ moves: [Move]) -> Move {
        while true {
            let square = Self.getSquare()
            // Check that the chosen square is actually one of the possible moves
            if let validMove = moves.first(where: { $0.square == square }) {
                return validMove
            }
            printError("  Can't place a \(self.disk) disk in square \(square)!")
        }
    }

    /// Ask human player for square coordinates.
    static func getSquare() -> Square {
        while true {
            print("  Give disk position (x,y): ", terminator: "")
            guard let input = readLine() else {
                printError("  Input failed. Please try again.")
                continue
            }
            let values = input.trimmingCharacters(in: .whitespaces).components(separatedBy: ",")
            if values.count == 2 {
                let x = Int(values[0]) ?? -1
                let y = Int(values[1]) ?? -1
                if x >= 0, y >= 0 {
                    return Square(x: x, y: y)
                }
            }
            printError("  Give coordinates in the form 'x,y'!")
        }
    }

    /// Return player type description string.
    func typeString() -> String {
        self.playerType.description
    }
}

/// Player can be controlled either by a human or computer.
enum PlayerType {
    case human
    case computer

    /// Check if the player is controlled by a human.
    func human() -> Bool {
        self == .human
    }

    /// Check if the player is controlled by a computer.
    func computer() -> Bool {
        self == .computer
    }
}

extension Player: CustomStringConvertible {
    var description: String {
        "\(self.disk) | \(self.typeString()) | Moves: \(self.roundsPlayed)"
    }
}

extension PlayerType: CustomStringConvertible {
    var description: String {
        switch self {
            case .human:
                "Human   "
            case .computer:
                "Computer"
        }
    }
}
