@testable import OthelloLib
import Testing

@Test func boardCanPlay() {
    let board = Board(size: 8)
    #expect(board.canPlay() == true)
}

@Test func boardResult() {
    let board = Board(size: 8)
    #expect(board.result() == .empty)

    let moves = board.possibleMoves(disk: .black)
    board.placeDisk(move: moves[0])

    #expect(board.result() == .black)
}

@Test func boardLogEntry() {
    let board8 = Board(size: 8)
    #expect(board8.logEntry() == "___________________________WB______BW___________________________")

    let board4 = Board(size: 4)
    #expect(board4.logEntry() == "_____WB__BW_____")

    let moves = board4.possibleMoves(disk: .black)
    board4.placeDisk(move: moves[0])
    #expect(board4.logEntry() == "____BBB__BW_____")
}
