//==========================================================
// Class Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

using Pastel;
using System;
using System.Drawing;
using System.Linq;

namespace Othello
{
    /// Gameplay loop and main logic.
    internal class Othello
    {
        public static int MIN_BOARD_SIZE = 4;
        public static int MAX_BOARD_SIZE = 10;

        private Board _board;
        private Player _playerBlack;
        private Player _playerWhite;
        private int _roundsPlayed;
        private int _gamesPlayed;
        private readonly int _boardSize;

        private Othello(int boardSize)
        {
            _board = new Board(boardSize);
            _boardSize = boardSize;
            _gamesPlayed = 0;
            _playerBlack = new Player(Disk.Black);
            _playerWhite = new Player(Disk.White);
            _roundsPlayed = 0;
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


        /// Keep making moves until both players can't make a move any more.
        private void GameLoop()
        {
            while (_board.CanPlay() && (_playerBlack.canPlay || _playerWhite.canPlay))
            {
                ++_roundsPlayed;
                Console.WriteLine($"\n=========== ROUND: {_roundsPlayed} ===========");
                _playerBlack.PlayOneMove(_board);
                Console.WriteLine("--------------------------------");
                _playerWhite.PlayOneMove(_board);
            }
            ++_roundsPlayed;
        }

        /// Print ending status and winner info.
        private void PrintResult()
        {
            Console.WriteLine("\n================================");
            ColorPrint.WriteLine("The game is finished!", Color.Green);
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
                    return Math.Max(Othello.MIN_BOARD_SIZE, Math.Min(boardSize, Othello.MAX_BOARD_SIZE));
                }
                ColorPrint.Error("give a valid number...");
            }
        }

        public static void Main(string[] args)
        {
            ColorPrint.WriteLine("OTHELLO GAME - C#", Color.Green);
            // Handle 'help' and 'version' arguments
            // TODO: version info
            if (args.Contains("--help") || args.Contains("-h"))
            {
                Console.WriteLine($"Othello C# {DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss")}\n");
                Console.WriteLine("USAGE: othello.exe [board size]\n");
                Console.WriteLine("Optional arguments:");
                Console.WriteLine("    -h | --help          Print usage and exit");
                Console.WriteLine("    -v | --version       Print version info and exit");
                Environment.Exit(1);
            }
            if (args.Contains("--version") || args.Contains("-v"))
            {
                Console.WriteLine($"Othello C# {DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss")}");
                Environment.Exit(0);
            }

            // try to read board size from command line args
            if (args.Length == 0 || !int.TryParse(args[0], out var boardSize))
            {
                // Otherwise ask user for board size
                boardSize = GetBoardSize();
            }
            else
            {
                if (boardSize < Othello.MIN_BOARD_SIZE || boardSize > Othello.MAX_BOARD_SIZE)
                {
                    ColorPrint.Error($"Unsupported board size: {boardSize}");
                    Environment.Exit(1);
                }
                Console.WriteLine($"Using board size: {boardSize}");
            }

            var game = new Othello(boardSize);
            game.Play();
        }
    }
}
