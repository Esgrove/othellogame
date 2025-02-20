using System.Text;

namespace Othello.Tests;

public class TestModels
{
    [Fact]
    public void Step_Equals()
    {
        var step1 = new Step(1, 1);
        var step2 = new Step(1, 1);

        Assert.Equal(step1, step2);
    }

    [Fact]
    public void Step_NotEquals()
    {
        var step1 = new Step(1, 1);
        var step2 = new Step(2, 2);

        Assert.NotEqual(step1, step2);
    }

    [Fact]
    public void Square_AddStep()
    {
        var square = new Square(4, 4);
        var step = new Step(-1, 1);

        var result = square + step;

        Assert.Equal(new Square(3, 5), result);
    }

    [Fact]
    public void Square_CompareToSquare()
    {
        var square1 = new Square(1, 2);
        var square2 = new Square(2, 1);

        Assert.True(square1.CompareTo(square2) < 0);
        Assert.True(square2.CompareTo(square1) > 0);
    }

    [Fact]
    public void Square_AdditionWithStep()
    {
        var square = new Square(4, 4);
        var step = new Step(-1, 1);
        var expectedSquare = new Square(3, 5);

        var resultSquare = square + step;

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
        var origin = new Square(1, 1);
        var step = new Step(stepX, stepY);
        var expectedResult = new Square(expectedX, expectedY);

        var result = origin + step;

        Assert.Equal(expectedResult, result);
    }
}
