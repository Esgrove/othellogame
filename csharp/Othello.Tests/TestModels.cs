namespace Othello.Tests
{
    public class TestModels
    {
        [Fact]
        public void Step_Equals()
        {
            Step step1 = new(1, 1);
            Step step2 = new(1, 1);

            Assert.Equal(step1, step2);
        }

        [Fact]
        public void Step_NotEquals()
        {
            Step step1 = new(1, 1);
            Step step2 = new(2, 2);

            Assert.NotEqual(step1, step2);
        }

        [Fact]
        public void Square_AddStep()
        {
            Square square = new(4, 4);
            Step step = new(-1, 1);

            Square result = square + step;

            Assert.Equal(new Square(3, 5), result);
        }

        [Fact]
        public void Square_CompareToSquare()
        {
            Square square1 = new(1, 2);
            Square square2 = new(2, 1);

            Assert.True(square1.CompareTo(square2) < 0);
            Assert.True(square2.CompareTo(square1) > 0);
        }

        [Fact]
        public void Square_AdditionWithStep()
        {
            Square square = new(4, 4);
            Step step = new(-1, 1);
            Square expectedSquare = new(3, 5);

            Square resultSquare = square + step;

            Assert.Equal(expectedSquare, resultSquare);
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
        public void Square_StepDirections(int stepX, int stepY, int expectedX, int expectedY)
        {
            Square origin = new(1, 1);
            Step step = new(stepX, stepY);
            Square expectedResult = new(expectedX, expectedY);

            Square result = origin + step;

            Assert.Equal(expectedResult, result);
        }
    }
}
