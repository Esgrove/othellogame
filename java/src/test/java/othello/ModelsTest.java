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

    @Test
    public void stepAddition() {
        Step result = new Step(0, 0).add(new Step(1, 1));
        assertEquals(new Step(1, 1), result);

        result = new Step(-1, 0).add(new Step(1, 0));
        assertEquals(new Step(0, 0), result);

        result = result.add(new Step(-1, -1));
        assertEquals(new Step(-1, -1), result);

        result = result.add(new Step(1, 1));
        assertEquals(new Step(0, 0), result);
    }

    @Test
    public void squareAddition() {
        Square result = new Square(4, 4).add(new Square(1, 1));
        assertEquals(new Square(5, 5), result);

        result = new Square(4, 4).add(new Square(0, 0));
        assertEquals(new Square(4, 4), result);

        result = new Square(4, 4).add(new Step(-1, 1));
        assertEquals(new Square(3, 5), result);

        result = result.add(new Square(0, 0));
        assertEquals(new Square(3, 5), result);

        Square another = new Square(-3, -2).add(new Square(2, 3));
        assertEquals(new Square(-1, 1), another);

        result = result.add(new Step(-1, -1));
        assertEquals(new Square(2, 4), result);

        result = result.add(new Step(-1, -1));
        assertEquals(new Square(1, 3), result);

        result = result.add(new Step(-1, -1));
        assertEquals(new Square(0, 2), result);

        result = result.add(new Step(-1, -1));
        assertEquals(new Square(-1, 1), result);

        result = result.add(new Step(1, -1));
        assertEquals(new Square(0, 0), result);

        result = result.add(new Step(-1, -1));
        assertEquals(new Square(-1, -1), result);
    }

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
        assertEquals("B", Disk.BLACK.boardChar());
        assertEquals("_", Disk.EMPTY.boardChar());
        assertEquals("W", Disk.WHITE.boardChar());
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
            Disk.BLACK,
            10,
            List.of(new Direction(new Step(1, 0), 10))
        );
        assertEquals("B:(3,2),10", b.logEntry());

        Move w = new Move(
            new Square(0, 0),
            Disk.WHITE,
            1,
            List.of(new Direction(new Step(1, 0), 1))
        );
        assertEquals("W:(0,0),1", w.logEntry());
    }
}
