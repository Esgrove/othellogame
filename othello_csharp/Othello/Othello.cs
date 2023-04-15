//==========================================================
// Class Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

using System;
using System.Drawing;
using Pastel;

namespace Othello
{
    /// Play Othello CLI game.
    internal class Othello
    {
        private Board _board;
        private Player _playerBlack;
        private Player _playerWhite;
        private int _roundsPlayed;
        private int _gamesPlayed;
        private readonly int _boardSize;

        private Othello(int boardSize)
        {
            _boardSize = boardSize;
            _board = new Board(_boardSize);
            _playerBlack = new Player(Disk.Black);
            _playerWhite = new Player(Disk.White);
            _roundsPlayed = 0;
            _gamesPlayed = 0;
        }


        /// Play one full game of Othello.
        private void Play()
        {
            while (true)
            {
                InitGame();
                GameLoop();
                PrintResult();
                if (!GetAnswer("\nWould you like to play again"))
                {
                    break;
                }
            }
        }

        /// Initialize game board and players for a new game.
        private void InitGame()
        {
            if (_gamesPlayed > 0)
            {
                _board = new Board(_boardSize);
                _playerBlack = new Player(Disk.Black);
                _playerWhite = new Player(Disk.White);
                _roundsPlayed = 0;
            }

            if (GetAnswer("Would you like to play against the computer"))
            {
                if (GetAnswer("Would you like to play as black or white", "b", "w"))
                {
                    _playerWhite.SetHuman(false);
                }
                else
                {
                    _playerBlack.SetHuman(false);
                }
            }

            Console.WriteLine("\nPlayers:".Pastel(Color.Silver));
            PrintStatus();
        }


        /// Keep making moves until both players can't make a move anymore.
        private void GameLoop()
        {
            while (_board.CanPlay() && (_playerBlack.CanPlay() || _playerWhite.CanPlay()))
            {
                ++_roundsPlayed;
                Console.WriteLine($"\n=========== ROUND: {_roundsPlayed} ===========");
                _playerBlack.PlayOneMove(_board);
                Console.WriteLine("--------------------------------");
                _playerWhite.PlayOneMove(_board);
            }
        }

        /// Print ending status and winner info.
        private void PrintResult()
        {
            Console.WriteLine("\n================================");
            ColorPrint.Write("The game is finished!\n", Color.Green);
            Console.WriteLine("Result:".Pastel(Color.Silver));
            PrintStatus();
            Console.WriteLine("");

            var winner = _board.Result();
            if (winner == Disk.Empty)
            {
                Console.WriteLine("The game ended in a tie...");
            }
            else
            {
                Console.WriteLine($"The winner is {winner.Name()}!");
            }
        }

        /// Print current board and player info.
        private void PrintStatus()
        {
            Console.WriteLine(_playerBlack);
            Console.WriteLine(_playerWhite);
            Console.WriteLine($"\n{_board}");
        }

        /// Ask a question with two options, and return bool from user answer.
        private static bool GetAnswer(string question, string yes = "y", string no = "n")
        {
            Console.Write($"{question} ({yes}/{no})? ");
            var ans = Console.ReadLine();
            return !string.IsNullOrEmpty(ans) && string.Equals(ans, yes, StringComparison.CurrentCultureIgnoreCase);
        }

        /// Ask and return the desired board size.
        private static int GetBoardSize()
        {
            while (true)
            {
                Console.Write("Choose board size (default is 8): ");
                if (int.TryParse(Console.ReadLine(), out var boardSize))
                {
                    return Math.Max(4, Math.Min(boardSize, 16));
                }
                ColorPrint.Error("give a valid number...");
            }
        }

        public static void Main(string[] args)
        {
            ColorPrint.Write("OTHELLO GAME - C#\n", Color.Green);
            // try to read board size from command line args
            if (args.Length == 0 || !int.TryParse(args[0], out var boardSize))
            {
                boardSize = GetBoardSize();
            }
            else
            {
                Console.WriteLine($"Using board size: {boardSize}");
            }

            var game = new Othello(boardSize);
            game.Play();
        }
    }
}
