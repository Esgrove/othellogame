package othello;

import java.util.List;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

import othello.Models.Direction;
import othello.Models.Disk;
import othello.Models.Move;
import othello.Models.Square;
import othello.Models.Step;

import static org.junit.jupiter.api.Assertions.*;

public class ModelsTest {

    @ParameterizedTest
    @CsvSource({
        "-1, -1, 0, 0",
        "-1,  0, 0, 1",
        "-1,  1, 0, 2",
        " 0, -1, 1, 0",
        " 0,  1, 1, 2",
        " 1, -1, 2, 0",
        " 1,  0, 2, 1",
        " 1,  1, 2, 2"
    })
    public void stepDirections(int stepX, int stepY, int expectedX, int expectedY) {
        Square origin = new Square(1, 1);
        Step step = new Step(stepX, stepY);
        Square expected = new Square(expectedX, expectedY);
        assertEquals(expected, origin.add(step));
    }

    @Test
    public void diskBoardChar() {
        assertEquals("B", Disk.BLACK.boardChar(false));
        assertEquals("_", Disk.EMPTY.boardChar(false));
        assertEquals("W", Disk.WHITE.boardChar(false));
    }

    @Test
    public void diskOpponent() {
        assertEquals(Disk.WHITE, Disk.BLACK.opponent());
        assertEquals(Disk.EMPTY, Disk.EMPTY.opponent());
        assertEquals(Disk.BLACK, Disk.WHITE.opponent());
    }

    @Test
    public void moveLogEntry() {
        Move b = new Move(
            new Square(3, 2),
            10,
            Disk.BLACK,
            List.of(new Direction(new Step(1, 0), 10))
        );
        assertEquals("B:(3,2),10", b.logEntry());

        Move w = new Move(
            new Square(0, 0),
            1,
            Disk.WHITE,
            List.of(new Direction(new Step(1, 0), 1))
        );
        assertEquals("W:(0,0),1", w.logEntry());
    }
}
