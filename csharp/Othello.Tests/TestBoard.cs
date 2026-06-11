namespace Othello.Tests {
    public class TestBoard {
        [Fact]
        public void BoardInitialization() {
            const int boardSize = 8;
            Board board = new(boardSize);
            Assert.Equal(boardSize, board.Size);
            // Check initial disk positions
            Assert.Equal(Disk.White, board.GetSquare(new Square(3, 3)));
            Assert.Equal(Disk.White, board.GetSquare(new Square(4, 4)));
            Assert.Equal(Disk.Black, board.GetSquare(new Square(3, 4)));
            Assert.Equal(Disk.Black, board.GetSquare(new Square(4, 3)));
            // Rest of the board should be empty
            Assert.Equal(Disk.Empty, board.GetSquare(new Square(0, 0)));
            Assert.Equal(Disk.Empty, board.GetSquare(new Square(7, 7)));
        }

        [Fact]
        public void CanPlay() {
            Board board = new(8);
            Assert.True(board.CanPlay());
        }

        [Fact]
        public void Scoring() {
            Board board = new(8);
            (int blackScore, int whiteScore) = board.PlayerScores();
            Assert.Equal(2, blackScore);
            Assert.Equal(2, whiteScore);
            Assert.Equal(0, board.Score());
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
