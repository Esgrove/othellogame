namespace Othello.Tests {
    public class TestModels {
        [Fact]
        public void StepAddition() {
            Step result = new Step(0, 0) + new Step(1, 1);
            Assert.Equal(new Step(1, 1), result);

            result = new Step(-1, 0) + new Step(1, 0);
            Assert.Equal(new Step(0, 0), result);

            result += new Step(-1, -1);
            Assert.Equal(new Step(-1, -1), result);

            result += new Step(1, 1);
            Assert.Equal(new Step(0, 0), result);
        }

        [Fact]
        public void SquareAddition() {
            Square result = new Square(4, 4) + new Square(1, 1);
            Assert.Equal(new Square(5, 5), result);

            result = new Square(4, 4) + new Square(0, 0);
            Assert.Equal(new Square(4, 4), result);

            result = new Square(4, 4) + new Step(-1, 1);
            Assert.Equal(new Square(3, 5), result);

            result += new Square(0, 0);
            Assert.Equal(new Square(3, 5), result);

            Square another = new Square(-3, -2) + new Square(2, 3);
            Assert.Equal(new Square(-1, 1), another);

            result += new Step(-1, -1);
            Assert.Equal(new Square(2, 4), result);

            result += new Step(-1, -1);
            Assert.Equal(new Square(1, 3), result);

            result += new Step(-1, -1);
            Assert.Equal(new Square(0, 2), result);

            result += new Step(-1, -1);
            Assert.Equal(new Square(-1, 1), result);

            result += new Step(1, -1);
            Assert.Equal(new Square(0, 0), result);

            result += new Step(-1, -1);
            Assert.Equal(new Square(-1, -1), result);
        }

        [Theory]
        [InlineData(-1, -1, 0, 0)]
        [InlineData(-1, 0, 0, 1)]
        [InlineData(-1, 1, 0, 2)]
        [InlineData(0, -1, 1, 0)]
        [InlineData(0, 1, 1, 2)]
        [InlineData(1, -1, 2, 0)]
        [InlineData(1, 0, 2, 1)]
        [InlineData(1, 1, 2, 2)]
        public void StepDirections(int stepX, int stepY, int expectedX, int expectedY) {
            Square origin = new(1, 1);
            Step step = new(stepX, stepY);
            Square expectedResult = new(expectedX, expectedY);

            Square result = origin + step;

            Assert.Equal(expectedResult, result);
        }

        [Fact]
        public void DiskBoardChar() {
            Assert.Equal("B", Disk.Black.BoardChar());
            Assert.Equal("_", Disk.Empty.BoardChar());
            Assert.Equal("W", Disk.White.BoardChar());
        }

        [Fact]
        public void DiskOpponent() {
            Assert.Equal(Disk.White, Disk.Black.Opponent());
            Assert.Equal(Disk.Empty, Disk.Empty.Opponent());
            Assert.Equal(Disk.Black, Disk.White.Opponent());
        }

        [Fact]
        public void MoveLogEntry() {
            Move b = new(
                new Square(3, 2),
                Disk.Black,
                10,
                [new Direction(new Step(1, 0), 10)]
            );
            Assert.Equal("B:(3,2),10", b.LogEntry());

            Move w = new(
                new Square(0, 0),
                Disk.White,
                1,
                [new Direction(new Step(1, 0), 1)]
            );
            Assert.Equal("W:(0,0),1", w.LogEntry());
        }

        [Fact]
        public void StepEquals() {
            Step step1 = new(1, 1);
            Step step2 = new(1, 1);

            Assert.Equal(step1, step2);
        }

        [Fact]
        public void StepNotEquals() {
            Step step1 = new(1, 1);
            Step step2 = new(2, 2);

            Assert.NotEqual(step1, step2);
        }

        [Fact]
        public void SquareCompareToSquare() {
            Square square1 = new(1, 2);
            Square square2 = new(2, 1);

            Assert.True(square1.CompareTo(square2) < 0);
            Assert.True(square2.CompareTo(square1) > 0);
        }
    }
}
