//==========================================================
// Class Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2025
//==========================================================

using System;
using System.Collections.Generic;
using System.CommandLine;
using System.Drawing;
using Pastel;

namespace Othello
{
    public class Program
    {
        /// Gameplay loop and main logic.
        internal class Othello
        {
            private const int MinBoardSize = 4;
            private const int MaxBoardSize = 10;
            private const int DefaultBoardSize = 8;

            private Board _board;
            private readonly Player _playerBlack;
            private readonly Player _playerWhite;
            private int _roundsPlayed;
            private int _gamesPlayed;
            private readonly Settings _settings;
            private readonly List<string> _gameLog = new();

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
                else if (_settings.UseDefaults)
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

                if (!_settings.CheckMode)
                {
                    Console.WriteLine("\nPlayers:".Pastel(Color.Silver));
                    PrintStatus();
                }
            }

            /// Keep making moves until both players can't make a move any more.
            private void GameLoop()
            {
                while (_board.CanPlay() && (_playerBlack.CanPlay || _playerWhite.CanPlay))
                {
                    ++_roundsPlayed;
                    PrintRoundHeader();
                    foreach (Player player in new[] { _playerBlack, _playerWhite })
                    {
                        string result = player.PlayOneMove(_board);
                        if (result != null)
                        {
                            _gameLog.Add($"{result};{_board.LogEntry()}");
                        }
                        PrintDivider();
                    }
                }
                ++_gamesPlayed;
                PrintGameEndFooter();
            }

            private string FormatGameLog()
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
                return formattedLog;
            }

            private void PrintRoundHeader()
            {
                if (!_settings.CheckMode)
                {
                    Console.WriteLine($"\n=========== ROUND: {_roundsPlayed} ===========");
                }
            }

            private void PrintDivider()
            {
                if (!_settings.CheckMode)
                {
                    Console.WriteLine("--------------------------------");
                }
            }

            private void PrintGameEndFooter()
            {
                if (!_settings.CheckMode)
                {
                    Console.WriteLine("\n================================");
                    ColorPrint.WriteLine("The game is finished!", Color.Green);
                }
            }

            private void PrintLog()
            {
                string formattedLog = FormatGameLog();
                if (!_settings.CheckMode)
                {
                    ColorPrint.WriteLine("Game log:", Color.Yellow);
                    Console.WriteLine(formattedLog);
                }
                string hexHash = Utils.CalculateSha256(formattedLog);
                Console.WriteLine(hexHash);
            }

            /// Print ending status and winner info.
            private void PrintResult()
            {
                Console.WriteLine("Result:".Pastel(Color.Silver));
                PrintStatus();
                Console.WriteLine("");

                Disk winner = _board.Result();
                Console.WriteLine(
                    winner == Disk.Empty
                        ? "The game ended in a tie...\n"
                        : $"The winner is {winner.Name()}!\n"
                );
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
                string answer = Console.ReadLine();
                return !string.IsNullOrEmpty(answer)
                    && string.Equals(answer, yes, StringComparison.CurrentCultureIgnoreCase);
            }

            /// Ask and return the desired board size.
            private static int GetBoardSize()
            {
                Console.Write($"Choose board size (default is {DefaultBoardSize}): ");
                if (int.TryParse(Console.ReadLine(), out int boardSize))
                {
                    if (boardSize is < MinBoardSize or > MaxBoardSize)
                    {
                        ColorPrint.Warn(
                            $"Limiting board size to valid range {MinBoardSize}...{MaxBoardSize}"
                        );
                    }
                    return Math.Max(MinBoardSize, Math.Min(boardSize, MaxBoardSize));
                }
                ColorPrint.Warn($"Invalid size, defaulting to {DefaultBoardSize}...");
                return DefaultBoardSize;
            }

            public static int Main(string[] args)
            {
                Argument<int?> size = new(
                    name: "size",
                    description: $"Optional board size ({MinBoardSize}..{MaxBoardSize})",
                    getDefaultValue: () => null
                );

                Option<bool> autoplay = new(
                    name: "--autoplay",
                    description: "Enable autoplay mode with computer control"
                );
                autoplay.AddAlias("-a");

                Option<bool> checkMode = new(
                    name: "--check",
                    description: "Autoplay and only print result"
                );
                checkMode.AddAlias("-c");

                Option<bool> useDefaultSettings = new(
                    name: "--default",
                    description: "Play with default settings"
                );
                useDefaultSettings.AddAlias("-d");

                Option<bool> showLog = new(name: "--log", description: "Show game log at the end");
                showLog.AddAlias("-l");

                Option<bool> hideHelpers = new(
                    name: "--no-helpers",
                    description: "Hide disk placement hints"
                );
                hideHelpers.AddAlias("-n");

                Option<bool> testMode = new(
                    name: "--test",
                    description: "Enable test mode with deterministic computer moves"
                );
                testMode.AddAlias("-t");

                Option<bool> version = new(name: "-v", description: "Print version and exit");
                // Can't add short alias since it conflicts with the default version flag.
                // Haven't found a way to override the default version flag.
                // Hopefully this would be supported in the future.
                // version.AddAlias("--version");

                RootCommand rootCommand = new("A simple Othello CLI game implementation in C#")
                {
                    size,
                    autoplay,
                    checkMode,
                    useDefaultSettings,
                    showLog,
                    hideHelpers,
                    testMode,
                    version,
                };

                rootCommand.SetHandler(
                    (
                        size,
                        autoplay,
                        checkMode,
                        useDefaultSettings,
                        hideHelpers,
                        showLog,
                        testMode,
                        version
                    ) =>
                    {
                        if (version)
                        {
                            Console.WriteLine($"Othello C# {Utils.VersionInfo()}");
                            Environment.Exit(0);
                        }

                        ColorPrint.WriteLine("OTHELLO GAME - C#", Color.Green);

                        int boardSize;
                        if (size != null)
                        {
                            // Try to read board size from command line args
                            boardSize = size.Value;
                            if (boardSize is < MinBoardSize or > MaxBoardSize)
                            {
                                ColorPrint.Error($"Unsupported board size: {boardSize}");
                                Environment.Exit(1);
                            }
                            Console.WriteLine($"Using board size: {boardSize}");
                        }
                        else if (autoplay || useDefaultSettings)
                        {
                            boardSize = DefaultBoardSize;
                        }
                        else
                        {
                            // Otherwise ask user for board size
                            boardSize = GetBoardSize();
                        }

                        Settings settings = new(
                            boardSize,
                            autoplay || checkMode,
                            checkMode,
                            !hideHelpers,
                            showLog || checkMode,
                            testMode || checkMode,
                            useDefaultSettings
                        );

                        new Othello(settings).Play();
                    },
                    size,
                    autoplay,
                    checkMode,
                    hideHelpers,
                    showLog,
                    testMode,
                    useDefaultSettings,
                    version
                );

                try
                {
                    return rootCommand.Invoke(args);
                }
                catch (OperationCanceledException)
                {
                    Console.WriteLine("\ncancelled...");
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
