using System;

namespace Othello
{
    internal class Othello
    {
        private Othello()
        {
            _board = null;
            _playerBlack = null;
            _playerWhite = null;
            _roundsPlayed = 0;
        }

        private void InitGame()
        {
            var boardSize = GetBoardSize();
            _board = new Board(boardSize);
            _playerBlack = new Player(Disk.Black);
            _playerWhite = new Player(Disk.White);
            _roundsPlayed = 0;

            if (GetAnswer("Would you like to play against the computer")) {
                if (GetAnswer("Would you like to play as black or white", "b", "w")) {
                    _playerWhite.SetHuman(false);
                } else {
                    _playerBlack.SetHuman(false);
                }
            }
            Console.WriteLine("Players:");
            _playerBlack.Print();
            _playerWhite.Print();
            Console.WriteLine();
            _board.Print();
        }

        private static bool GetAnswer(string message, string yes="y", string no="n")
        {
            Console.Write($"{message} ({yes}/{no})? ");
            var ans = Console.ReadLine();
            if (!string.IsNullOrEmpty(ans)) {
                return ans.ToLower() == yes;
            }
            return false;
        }

        private static int GetBoardSize()
        {
            Console.Write("Choose board size (default is 8): ");
            var size = Convert.ToInt32(Console.ReadLine());
            size = Math.Max(4, Math.Min(size, 8));
            return size;
        }

        private void Play()
        {
            while (true) {
                InitGame();
                while (_playerBlack.CanPlay() || _playerWhite.CanPlay()) {
                    ++_roundsPlayed;
                    Console.WriteLine($"=========== ROUND: {_roundsPlayed} ===========");
                    _playerBlack.PlayOneMove(_board);
                    _playerWhite.PlayOneMove(_board);
                    _board.PrintScore();
                }

                ColorPrint.Write("The game is finished!\n", ConsoleColor.Green);
                Console.WriteLine($"total rounds played: {_roundsPlayed}");
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

                if (!GetAnswer("\nWould you like to play again")) {
                    break;
                }
            }
        }

        private Board _board;
        private Player _playerBlack;
        private Player _playerWhite;
        private int _roundsPlayed;

        public static void Main(string[] args)
        {
            ColorPrint.Write("OTHELLO GAME - C#\n", ConsoleColor.Green);
            var othello = new Othello();
            othello.Play();
        }
    }
}