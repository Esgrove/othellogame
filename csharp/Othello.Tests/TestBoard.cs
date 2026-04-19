namespace Othello.Tests {
    public class TestBoard {
        [Fact]
        public void CanPlay() {
            Board board = new(8);
            Assert.True(board.CanPlay());
        }

        [Fact]
        public void GameResult() {
            Board board = new(8);
            Disk result = board.Result();
            Assert.Equal(Disk.Empty, result);

            var moves = board.PossibleMoves(Disk.Black);
            board.PlaceDisk(moves[0]);

            result = board.Result();
            Assert.Equal(Disk.Black, result);
        }

        [Fact]
        public void LogEntry() {
            Board board8 = new(8);
            Assert.Equal(
                "___________________________WB______BW___________________________",
                board8.LogEntry()
            );

            Board board4 = new(4);
            Assert.Equal("_____WB__BW_____", board4.LogEntry());

            var moves = board4.PossibleMoves(Disk.Black);
            board4.PlaceDisk(moves[0]);
            Assert.Equal("____BBB__BW_____", board4.LogEntry());
        }
    }
}
