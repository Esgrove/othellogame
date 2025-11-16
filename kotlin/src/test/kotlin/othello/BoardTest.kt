package othello

import kotlin.test.Test
import kotlin.test.assertEquals
import kotlin.test.assertTrue

class BoardTest {
    @Test
    fun boardInitialization() {
        val boardSize = 8
        val board = Board(boardSize)

        assertEquals(boardSize, board.size)

        // Check initial disk positions
        assertEquals(Disk.White, board.getSquare(Square(3, 3)))
        assertEquals(Disk.White, board.getSquare(Square(4, 4)))
        assertEquals(Disk.Black, board.getSquare(Square(3, 4)))
        assertEquals(Disk.Black, board.getSquare(Square(4, 3)))

        // Rest of the board should be empty
        assertEquals(Disk.Empty, board.getSquare(Square(0, 0)))
        assertEquals(Disk.Empty, board.getSquare(Square(7, 7)))
    }

    @Test
    fun canPlay() {
        val board = Board(8)
        assertTrue(board.canPlay())
    }

    @Test
    fun scoring() {
        val board = Board(8)
        val (blackScore, whiteScore) = board.playerScores()
        assertEquals(2, blackScore)
        assertEquals(2, whiteScore)
        assertEquals(0, board.score())
    }

    @Test
    fun gameResult() {
        val board = Board(8)

        var result = board.result()
        assertEquals(Disk.Empty, result)

        val moves = board.possibleMoves(Disk.Black)
        board.placeDisk(moves[0])

        result = board.result()
        assertEquals(Disk.Black, result)
    }

    @Test
    fun logEntry() {
        val board8 = Board(8)
        val log8 = board8.logEntry()
        assertEquals(
            "___________________________WB______BW___________________________",
            log8,
        )

        val board4 = Board(4)
        val log4Initial = board4.logEntry()
        assertEquals("_____WB__BW_____", log4Initial)

        val moves = board4.possibleMoves(Disk.Black)
        board4.placeDisk(moves[0])
        val log4AfterMove = board4.logEntry()
        assertEquals("____BBB__BW_____", log4AfterMove)
    }
}
