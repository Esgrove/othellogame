@testable import OthelloLib
import Testing

@Test func boardInitialization() {
    let boardSize = 8
    let board = Board(size: boardSize)
    #expect(board.size == boardSize)
    // Check initial disk positions
    #expect(board.getSquare(Square(x: 3, y: 3)) == .white)
    #expect(board.getSquare(Square(x: 4, y: 4)) == .white)
    #expect(board.getSquare(Square(x: 3, y: 4)) == .black)
    #expect(board.getSquare(Square(x: 4, y: 3)) == .black)
    // Rest of the board should be empty
    #expect(board.getSquare(Square(x: 0, y: 0)) == .empty)
    #expect(board.getSquare(Square(x: 7, y: 7)) == .empty)
}

@Test func canPlay() {
    let board = Board(size: 8)
    #expect(board.canPlay() == true)
}

@Test func scoring() {
    let board = Board(size: 8)
    let (blackScore, whiteScore) = board.playerScores()
    #expect(blackScore == 2)
    #expect(whiteScore == 2)
    #expect(board.score() == 0)
}

@Test func gameResult() {
    let board = Board(size: 8)
    #expect(board.result() == .empty)

    let moves = board.possibleMoves(disk: .black)
    board.placeDisk(moves[0])

    #expect(board.result() == .black)
}

@Test func logEntry() {
    let board8 = Board(size: 8)
    #expect(board8.logEntry() == "___________________________WB______BW___________________________")

    let board4 = Board(size: 4)
    #expect(board4.logEntry() == "_____WB__BW_____")

    let moves = board4.possibleMoves(disk: .black)
    board4.placeDisk(moves[0])
    #expect(board4.logEntry() == "____BBB__BW_____")
}
