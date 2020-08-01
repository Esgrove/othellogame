using System;

namespace Othello
{
    internal class Othello
    {
        private Othello() {
            _board = null;
            _playerBlack = null;
            _playerWhite = null;
            _roundsPlayed = 0;
        }

        private void InitGame() {
            var boardSize = GetBoardSize();
            _board = new Board(boardSize);
            _playerBlack = new Player(Disk.Black);
            _playerWhite = new Player(Disk.White);
            _roundsPlayed = 0;

            if (GetAnswer("Would you like to play against the computer")) {
                if (GetAnswer("Would you like to play as black or white", "b", "w")) {
                    _playerWhite.SetPlayerType(false);
                } else {
                    _playerBlack.SetPlayerType(false);
                }
            }
            Console.WriteLine("Players:");
            _playerBlack.Print();
            _playerWhite.Print();
            Console.WriteLine();
            _board.Print();
        }

        private static bool GetAnswer(string message, string yes = "y", string no = "n") {
            Console.Write($"{message} ({yes}/{no})? ");
            var ans = Console.ReadLine();
            return !string.IsNullOrEmpty(ans) && ans.ToLower() == yes.ToLower();
        }

        private static int GetBoardSize() {
            Console.Write("Choose board size (default is 8): ");
            var size = Convert.ToInt32(Console.ReadLine());
            size = Math.Max(4, Math.Min(size, 8));
            return size;
        }

        public void Play() {
            while (true) {
                InitGame();
                GameLoop();
                ShowResult();
                if (!GetAnswer("\nWould you like to play again")) {
                    break;
                }
            }
        }

        private void GameLoop() {
            while (_board.CanPlay() && (_playerBlack.CanPlay() || _playerWhite.CanPlay())) {
                ++_roundsPlayed;
                Console.WriteLine($"=========== ROUND: {_roundsPlayed} ===========");
                _playerBlack.PlayOneMove(_board);
                Console.WriteLine("--------------------------------");
                _playerWhite.PlayOneMove(_board);
                ++_roundsPlayed;
            }
        }

        private void ShowResult() {
            Console.WriteLine("\n================================");
            ColorPrint.Write("The game is finished!\n", ConsoleColor.Green);
            Console.WriteLine("Result:");
            _board.Print();
            _playerBlack.Print();
            _playerWhite.Print();

            var winner = _board.Result();
            if (winner == Disk.Empty) {
                Console.WriteLine("The game ended in a tie...");
            } else {
                ColorPrint.Write($"The {winner} player won!", winner.DiskColor());
            }
        }

        private Board _board;
        private Player _playerBlack;
        private Player _playerWhite;
        private int _roundsPlayed;

        public static void Main(string[] args) {
            ColorPrint.Write("OTHELLO GAME - C#\n", ConsoleColor.Green);
            var game = new Othello();
            game.Play();
        }
    }
}