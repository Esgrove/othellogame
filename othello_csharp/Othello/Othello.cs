//==========================================================
// Class Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

using System;
using System.CommandLine;
using System.CommandLine.Invocation;
using Pastel;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.IO;

namespace Othello
{
    public class Program
    {
        /// Gameplay loop and main logic.
        internal class Othello
        {
            public static int MIN_BOARD_SIZE = 4;
            public static int MAX_BOARD_SIZE = 10;
            public static int DEFAULT_BOARD_SIZE = 8;

            private Board _board;
            private Player _playerBlack;
            private Player _playerWhite;
            private int _roundsPlayed;
            private int _gamesPlayed;
            private readonly Settings _settings;
            private List<string> _gameLog = new();

            private Othello(Settings settings)
            {
                _board = new Board(settings.BoardSize);
                _settings = settings;
                _gamesPlayed = 0;
                _playerBlack = Player.Black(settings.ToPlayerSettings());
                _playerWhite = Player.White(settings.ToPlayerSettings());
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
                    if (_settings.ShowLog)
                    {
                        PrintLog();
                    }
                    if (_settings.AutoplayMode || !GetAnswer("Would you like to play again"))
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
                    _board = new Board(_settings.BoardSize);
                    _playerBlack.Reset();
                    _playerWhite.Reset();
                    _roundsPlayed = 0;
                    _gameLog.Clear();
                }

                if (_settings.AutoplayMode)
                {
                    // Computer plays both
                    _playerWhite.SetHuman(false);
                    _playerBlack.SetHuman(false);
                }
                else if (_settings.QuickStart)
                {
                    // Default: play as black against white computer player
                    _playerWhite.SetHuman(false);
                }
                else if (GetAnswer("Would you like to play against the computer"))
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
                    foreach (Player player in new Player[] { _playerBlack, _playerWhite })
                    {
                        var result = player.PlayOneMove(_board);
                        if (result != null)
                        {
                            _gameLog.Add($"{result};{_board.ToLogEntry()}");
                        }
                        Console.WriteLine("--------------------------------");
                    }
                }
                ++_gamesPlayed;
            }

            private void PrintLog()
            {
                string formattedLog = "";
                int index = 1;
                foreach (string line in _gameLog)
                {
                    formattedLog += $"{index++:00}: {line}";
                    if (index <= _gameLog.Count)
                    {
                        formattedLog += "\n";
                    }
                }

                var hexHash = Utils.CalculateSHA256(formattedLog);
                ColorPrint.WriteLine("Game log:", Color.Yellow);
                Console.WriteLine(formattedLog);
                Console.WriteLine(hexHash);
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
                    Console.WriteLine("The game ended in a tie...\n");
                }
                else
                {
                    Console.WriteLine($"The winner is {winner.Name()}!\n");
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
                return !string.IsNullOrEmpty(ans)
                    && string.Equals(ans, yes, StringComparison.CurrentCultureIgnoreCase);
            }

            /// Ask and return the desired board size.
            private static int GetBoardSize()
            {
                Console.Write($"Choose board size (default is {DEFAULT_BOARD_SIZE}): ");
                if (int.TryParse(Console.ReadLine(), out var boardSize))
                {
                    if (boardSize < Othello.MIN_BOARD_SIZE || boardSize > Othello.MAX_BOARD_SIZE)
                    {
                        ColorPrint.Warn($"Limiting board size to valid range {Othello.MIN_BOARD_SIZE}...{Othello.MAX_BOARD_SIZE}");
                    }
                    return Math.Max(
                        Othello.MIN_BOARD_SIZE,
                        Math.Min(boardSize, Othello.MAX_BOARD_SIZE)
                    );
                }
                ColorPrint.Warn($"Invalid size, defaulting to {Othello.DEFAULT_BOARD_SIZE}...");
                return Othello.DEFAULT_BOARD_SIZE;
            }

            public static int Main(string[] args)
            {
                var size = new Argument<int?>
                    (name: "size",
                    description: "Optional board size",
                    getDefaultValue: () => null);

                var autoplay = new Option<bool>(
                   name: "--autoplay",
                   description: "Enable autoplay mode");
                autoplay.AddAlias("-a");

                var useDefaultSettings = new Option<bool>(
                   name: "--default",
                   description: "Play with default settings");
                useDefaultSettings.AddAlias("-d");

                var showLog = new Option<bool>(
                   name: "--log",
                   description: "Show log after a game");
                showLog.AddAlias("-l");

                var hideHelpers = new Option<bool>(
                   name: "--no-helpers",
                   description: "Hide disk placement hints");
                hideHelpers.AddAlias("-n");

                var testMode = new Option<bool>(
                   name: "--test",
                   description: "Enable test mode");
                testMode.AddAlias("-t");

                var version = new Option<bool>(
                   name: "-v",
                   description: "Print version and exit");

                var rootCommand = new RootCommand("A simple Othello CLI game implementation")
                {
                    size,
                    autoplay,
                    useDefaultSettings,
                    showLog,
                    hideHelpers,
                    testMode,
                    version
                };

                ColorPrint.WriteLine("OTHELLO GAME - C#", Color.Green);

                rootCommand.SetHandler((size, autoplay, useDefaultSettings, showLog, hideHelpers, testMode, version) =>
                    {
                        if (version)
                        {
                            Console.WriteLine($"Othello C# {Utils.VersionInfo()}");
                            Environment.Exit(0);
                        }

                        int boardSize;
                        if (size != null)
                        {
                            boardSize = size.Value;
                            if (boardSize < Othello.MIN_BOARD_SIZE || boardSize > Othello.MAX_BOARD_SIZE)
                            {
                                ColorPrint.Error($"Unsupported board size: {boardSize}");
                                Environment.Exit(1);
                            }
                            Console.WriteLine($"Using board size: {boardSize}");

                        }
                        else if (autoplay || useDefaultSettings)
                        {
                            boardSize = DEFAULT_BOARD_SIZE;
                        }
                        else
                        {
                            // Ask user for board size
                            boardSize = GetBoardSize();
                        }

                        Settings settings = new(boardSize, autoplay, useDefaultSettings, showLog, !hideHelpers, testMode);

                        var game = new Othello(settings);
                        game.Play();
                    },
                    size, autoplay, useDefaultSettings, showLog, hideHelpers, testMode, version);

                try
                {
                    return rootCommand.Invoke(args);
                }
                catch (OperationCanceledException)
                {
                    Console.WriteLine("\naborted...");
                    return 1;
                }
                catch (Exception ex)
                {
                    ColorPrint.Error($"An exception occurred: {ex.Message}");
                    return 1;
                }
            }
        }
    }
}
