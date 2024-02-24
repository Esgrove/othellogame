using System.Text;

namespace Othello.Tests;

public class TestUtils
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

    [Fact]
    public void CalculateSHA256_Basic()
    {
        var input = "test";
        var expectedHash = "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08";

        var result = Utils.CalculateSHA256(input);

        Assert.Equal(expectedHash, result);
    }

    [Fact]
    public void CalculateSHA256_log_line()
    {
        var input = "01: B:(0,1),1;____BBB__BW_____";
        var expectedHash = "d087cd890b81b7153cc870fba7372396e91cdcd4b6d13b82bbc88b5a4eb889d1";

        var result = Utils.CalculateSHA256(input);

        Assert.Equal(expectedHash, result);
    }

    [Fact]
    public void CalculateSHA256_full_log()
    {
        StringBuilder testLog = new StringBuilder();
        testLog.Append("01: B:(0,1),1;____BBB__BW_____\n");
        testLog.Append("02: W:(0,0),1;W___BWB__BW_____\n");
        testLog.Append("03: B:(1,0),1;WB__BBB__BW_____\n");
        testLog.Append("04: W:(0,2),2;WB__WBB_WWW_____\n");
        testLog.Append("05: B:(0,3),1;WB__WBB_WBW_B___\n");
        testLog.Append("06: W:(2,0),3;WWW_WWW_WBW_B___\n");
        testLog.Append("07: B:(3,0),1;WWWBWWB_WBW_B___\n");
        testLog.Append("08: W:(1,3),1;WWWBWWB_WWW_BW__\n");
        testLog.Append("09: B:(2,3),2;WWWBWWB_WWB_BBB_\n");
        testLog.Append("10: W:(3,2),2;WWWBWWW_WWWWBBB_\n");
        testLog.Append("11: B:(3,1),1;WWWBWWWBWWBWBBB_\n");
        testLog.Append("12: W:(3,3),1;WWWBWWWBWWWWBBBW");

        var input = testLog.ToString();

        var expectedHash = "ad4679949110ed7997aa1bf09441a7eb491b498189c03685ffdd6ddbb70e3c22";

        var result = Utils.CalculateSHA256(input);

        Assert.Equal(expectedHash, result);
    }
}
