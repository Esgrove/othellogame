//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2026
//==========================================================

import Foundation

/// Defines one player that can be either human or computer controlled.
class Player {
    var canPlay: Bool = true
    var disk: Disk
    var playerType: PlayerType = .human
    var roundsPlayed: Int = 0
    var settings: PlayerSettings

    init(_ disk: Disk, _ settings: PlayerSettings) {
        self.disk = disk
        self.settings = settings
    }

    /// Shorthand to initialize a new player for black disks.
    static func black(_ settings: PlayerSettings) -> Player {
        Player(Disk.black, settings)
    }

    /// Shorthand to initialize a new player for white disks.
    static func white(_ settings: PlayerSettings) -> Player {
        Player(Disk.white, settings)
    }

    /// Play one round as this player.
    func playOneMove(board: inout Board) -> String? {
        if !self.settings.checkMode {
            print("Turn: \(self.disk)")
        }
        let moves = board.possibleMoves(disk: self.disk)
        if !moves.isEmpty {
            self.canPlay = true
            if self.human() && self.settings.showHelpers && !self.settings.checkMode {
                board.printPossibleMoves(moves)
            }
            let chosenMove = self.human() ? self.getHumanMove(moves) : self.getComputerMove(moves)
            board.placeDisk(move: chosenMove)
            if !self.settings.testMode {
                board.printScore()
                Thread.sleep(forTimeInterval: 1)
            }
            self.roundsPlayed += 1
            return chosenMove.logEntry()
        }
        self.canPlay = false
        if !self.settings.checkMode {
            print("  No moves available...".yellow())
        }
        return nil
    }

    /// Reset player status for a new game.
    func reset() {
        self.canPlay = true
        self.roundsPlayed = 0
    }

    /// Returns true if the player is human.
    func human() -> Bool {
        self.playerType.isHuman()
    }

    /// Returns true if the player is controlled by computer.
    func computer() -> Bool {
        self.playerType.isComputer()
    }

    /// Set player to be controlled by human.
    func setHuman() {
        self.playerType = .human
    }

    /// Set player to be controlled by computer.
    func setComputer() {
        self.playerType = .computer
    }

    /// Set the player type.
    func setPlayerType(_ playerType: PlayerType) {
        self.playerType = playerType
    }

    /// Return move chosen by computer.
    func getComputerMove(_ moves: [Move]) -> Move {
        if !self.settings.checkMode {
            print("  Computer plays...")
        }
        let move: Move
        if self.settings.testMode {
            move = moves[0]
        } else {
            let seconds = Double.random(in: 1 ... 2)
            Thread.sleep(forTimeInterval: seconds)
            move = moves.randomElement()!
        }

        if !self.settings.checkMode {
            print("  \(move.square) -> \(move.value)")
        }
        return move
    }

    /// Return move chosen by a human player.
    func getHumanMove(_ moves: [Move]) -> Move {
        while true {
            let square = self.getSquare()
            if let move = moves.first(where: { $0.square == square }) {
                return move
            }
            printError("  Can't place a \(self.disk) disk in square \(square)")
        }
    }

    /// Ask human player for square coordinates.
    func getSquare() -> Square {
        while true {
            print("  Give disk position (x,y): ", terminator: "")
            if let pos = readLine() {
                let coordinates = pos.components(separatedBy: ",").compactMap { Int($0) }
                if coordinates.count == 2 {
                    return Square(coordinates[0], coordinates[1])
                }
            }
            printError("  Give coordinates in the form 'x,y'")
        }
    }

    /// Return player type description string.
    func typeString() -> String {
        self.playerType.description
    }
}

extension Player: CustomStringConvertible {
    var description: String {
        "\(self.disk) | \(self.typeString()) | Moves: \(self.roundsPlayed)"
    }
}
