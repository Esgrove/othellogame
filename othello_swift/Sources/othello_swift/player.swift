//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2023
//==========================================================

import Foundation

class Player {
    var canPlay: Bool = true
    var disk: Disk
    var isHuman: Bool = true
    var roundsPlayed: Int = 0
    var showHelpers: Bool = true

    init(_ disk: Disk) {
        self.disk = disk
    }

    /// Play one round as this player.
    func playOneMove(board: inout Board) {
        print("Turn: \(self.disk)")
        let moves = board.possibleMoves(disk: self.disk)
        if !moves.isEmpty {
            self.canPlay = true
            if self.isHuman && self.showHelpers {
                board.printPossibleMoves(moves)
            }
            let chosenMove = self.isHuman ? self.getHumanMove(moves) : self.getComputerMove(moves)
            board.placeDisk(move: chosenMove)
            board.printScore()
            self.roundsPlayed += 1
        } else {
            self.canPlay = false
            print("  No moves available...".yellow())
        }
    }

    /// Set player to be controlled by human or computer.
    func setHuman(_ isHuman: Bool) {
        self.isHuman = isHuman
    }

    /// Return move chosen by computer.
    func getComputerMove(_ moves: [Move]) -> Move {
        print("  Computer plays...")
        let seconds = Double.random(in: 1 ... 2)
        Thread.sleep(forTimeInterval: seconds)
        let move = moves.randomElement()!
        print("  \(move.square) -> \(move.value)")
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

    func typeString() -> String {
        self.isHuman ? "Human   " : "Computer"
    }
}

extension Player: CustomStringConvertible {
    var description: String {
        "\(self.disk) | \(self.typeString()) | Moves: \(self.roundsPlayed)"
    }
}
