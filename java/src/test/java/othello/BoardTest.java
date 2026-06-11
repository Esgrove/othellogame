package othello;

import org.junit.jupiter.api.Test;

import othello.Models.Disk;
import othello.Models.Square;
import othello.Utils.IntPair;

import static org.junit.jupiter.api.Assertions.*;

public class BoardTest {

    @Test
    public void boardInitialization() {
        int boardSize = 8;
        Board board = new Board(boardSize);
        assertEquals(boardSize, board.size);
        // Check initial disk positions
        assertEquals(Disk.WHITE, board.getSquare(new Square(3, 3)));
        assertEquals(Disk.WHITE, board.getSquare(new Square(4, 4)));
        assertEquals(Disk.BLACK, board.getSquare(new Square(3, 4)));
        assertEquals(Disk.BLACK, board.getSquare(new Square(4, 3)));
        // Rest of the board should be empty
        assertEquals(Disk.EMPTY, board.getSquare(new Square(0, 0)));
        assertEquals(Disk.EMPTY, board.getSquare(new Square(7, 7)));
    }

    @Test
    public void canPlay() {
        Board board = new Board(8);
        assertTrue(board.canPlay());
    }

    @Test
    public void scoring() {
        Board board = new Board(8);
        IntPair scores = board.playerScores();
        assertEquals(2, scores.first());
        assertEquals(2, scores.second());
        assertEquals(0, board.score());
    }

    @Test
    public void gameResult() {
        Board board = new Board(8);
        Disk result = board.result();
        assertEquals(Disk.EMPTY, result);

        var moves = board.possibleMoves(Disk.BLACK);
        board.placeDisk(moves.getFirst());

        result = board.result();
        assertEquals(Disk.BLACK, result);
    }

    @Test
    public void logEntry() {
        Board board8 = new Board(8);
        assertEquals(
            "___________________________WB______BW___________________________",
            board8.logEntry()
        );

        Board board4 = new Board(4);
        assertEquals("_____WB__BW_____", board4.logEntry());

        var moves = board4.possibleMoves(Disk.BLACK);
        board4.placeDisk(moves.getFirst());
        assertEquals("____BBB__BW_____", board4.logEntry());
    }
}
