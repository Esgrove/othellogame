package othello

import kotlin.test.Test
import kotlin.test.assertEquals

class ModelsTest {
    @Test
    fun stepAddition() {
        var result = Step(0, 0) + Step(1, 1)
        assertEquals(Step(1, 1), result)

        result = Step(-1, 0) + Step(1, 0)
        assertEquals(Step(0, 0), result)

        result += Step(-1, -1)
        assertEquals(Step(-1, -1), result)

        result += Step(1, 1)
        assertEquals(Step(0, 0), result)
    }

    @Test
    fun squareAddition() {
        var result = Square(4, 4) + Square(1, 1)
        assertEquals(Square(5, 5), result)

        result = Square(4, 4) + Square(0, 0)
        assertEquals(Square(4, 4), result)

        result = Square(4, 4) + Step(-1, 1)
        assertEquals(Square(3, 5), result)

        result += Square(0, 0)
        assertEquals(Square(3, 5), result)

        val another = Square(-3, -2) + Square(2, 3)
        assertEquals(Square(-1, 1), another)

        result += Step(-1, -1)
        assertEquals(Square(2, 4), result)

        result += Step(-1, -1)
        assertEquals(Square(1, 3), result)

        result += Step(-1, -1)
        assertEquals(Square(0, 2), result)

        result += Step(-1, -1)
        assertEquals(Square(-1, 1), result)

        result += Step(1, -1)
        assertEquals(Square(0, 0), result)

        result += Step(-1, -1)
        assertEquals(Square(-1, -1), result)
    }

    @Test
    fun stepDirections() {
        val origin = Square(1, 1)
        val stepDirections = arrayOf(
            Step(-1, -1),
            Step(-1, 0),
            Step(-1, 1),
            Step(0, -1),
            Step(0, 1),
            Step(1, -1),
            Step(1, 0),
            Step(1, 1),
        )

        val expectedResults = arrayOf(
            Square(0, 0),
            Square(0, 1),
            Square(0, 2),
            Square(1, 0),
            Square(1, 2),
            Square(2, 0),
            Square(2, 1),
            Square(2, 2),
        )

        stepDirections.forEachIndexed { i, step ->
            val result = origin + step
            assertEquals(expectedResults[i], result, "Step direction: $step")
        }
    }

    @Test
    fun diskBoardChar() {
        assertEquals("B", Disk.Black.boardChar(color = false))
        assertEquals("_", Disk.Empty.boardChar(color = false))
        assertEquals("W", Disk.White.boardChar(color = false))
    }

    @Test
    fun diskOpponent() {
        assertEquals(Disk.White, Disk.Black.opponent())
        assertEquals(Disk.Empty, Disk.Empty.opponent())
        assertEquals(Disk.Black, Disk.White.opponent())
    }

    @Test
    fun moveLogEntry() {
        val b = Move(
            square = Square(3, 2),
            disk = Disk.Black,
            value = 10,
            directions = listOf(
                Direction(
                    step = Step(1, 0),
                    count = 10,
                ),
            ),
        )
        assertEquals("B:(3,2),10", b.logEntry())

        val w = Move(
            square = Square(0, 0),
            disk = Disk.White,
            value = 1,
            directions = listOf(
                Direction(
                    step = Step(1, 0),
                    count = 1,
                ),
            ),
        )
        assertEquals("W:(0,0),1", w.logEntry())
    }
}
