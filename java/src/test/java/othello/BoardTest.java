package othello;

import org.junit.jupiter.api.Test;

import othello.Models.Disk;

import static org.junit.jupiter.api.Assertions.*;

public class BoardTest {

    @Test
    public void canPlay() {
        Board board = new Board(8);
        assertTrue(board.canPlay());
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
