//==========================================================
// Class Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

using System;
using System.Collections.Generic;
using System.Linq;

namespace Othello;

/// <summary>Gameplay loop and main logic.</summary>
internal sealed class Othello {
    private Board _board;
    private readonly Settings _settings;
    private readonly Player _playerBlack;
    private readonly Player _playerWhite;
    private readonly List<string> _gameLog = [];
    private int _gamesPlayed;
    private int _roundsPlayed;

    /// <summary>Initialize Othello game.</summary>
    public Othello(Settings settings) {
        _board = new Board(settings.BoardSize);
        _settings = settings;
        _playerBlack = Player.Black(settings.ToPlayerSettings());
        _playerWhite = Player.White(settings.ToPlayerSettings());
    }

    /// <summary>Play one full game of Othello.</summary>
    public void Play() {
        while (true) {
            InitGame();
            GameLoop();
            PrintResult();
            if (_settings.ShowLog) {
                PrintLog();
            }
            if (_settings.AutoplayMode || !GetAnswer("Would you like to play again")) {
                break;
            }
        }
    }

    /// <summary>Initialize game board and players for a new game.</summary>
    private void InitGame() {
        // Re-use existing objects instead of initializing new ones
        if (_gamesPlayed > 0) {
            _board = new Board(_settings.BoardSize);
            _playerBlack.Reset();
            _playerWhite.Reset();
            _roundsPlayed = 0;
            _gameLog.Clear();
        }
        if (_settings.AutoplayMode) {
            // Computer plays both
            _playerBlack.SetComputer();
            _playerWhite.SetComputer();
        } else if (_settings.UseDefaults) {
            // Default: play as black against white computer player
            _playerWhite.SetComputer();
        } else if (GetAnswer("Would you like to play against the computer")) {
            if (GetAnswer("Would you like to play as black or white", "b", "w")) {
                _playerWhite.SetComputer();
            } else {
                _playerBlack.SetComputer();
            }
        }
        if (!_settings.CheckMode) {
            ColorPrint.PrintBold("\nPlayers:");
            PrintStatus();
        }
    }

    /// <summary>Keep making moves until both players can't make a move any more.</summary>
    private void GameLoop() {
        while (_board.CanPlay() && (_playerBlack.CanPlay || _playerWhite.CanPlay)) {
            ++_roundsPlayed;
            PrintRoundHeader();
            foreach (Player player in new[] { _playerBlack, _playerWhite }) {
                string? result = player.PlayOneMove(_board);
                if (result != null) {
                    _gameLog.Add($"{result};{_board.LogEntry()}");
                }
                if (!_settings.CheckMode) {
                    Console.WriteLine("--------------------------------");
                }
            }
        }
        ++_gamesPlayed;
        PrintGameEndFooter();
    }

    /// <summary>Format game log with line numbers for each move.</summary>
    private string FormatGameLog() {
        return string.Join(
            "\n",
            _gameLog.Select((line, index) => $"{index + 1:00}: {line}")
        );
    }

    /// <summary>Print header for the current round.</summary>
    private void PrintRoundHeader() {
        if (!_settings.CheckMode) {
            ColorPrint.PrintBold($"\n=========== ROUND: {_roundsPlayed} ===========");
        }
    }

    /// <summary>Print footer after the game has ended.</summary>
    private void PrintGameEndFooter() {
        if (!_settings.CheckMode) {
            ColorPrint.PrintBold("\n================================");
            ColorPrint.PrintGreenBold("The game is finished!\n");
        }
    }

    /// <summary>Print game log which shows all moves made and the game board state after each move.</summary>
    private void PrintLog() {
        string formattedLog = FormatGameLog();
        if (!_settings.CheckMode) {
            ColorPrint.PrintYellowBold("Game log:");
            Console.WriteLine(formattedLog);
        }
        string hexHash = Utils.CalculateSha256(formattedLog);
        Console.WriteLine(hexHash);
    }

    /// <summary>Print ending status and winner info.</summary>
    private void PrintResult() {
        ColorPrint.PrintBold("Result:");
        PrintStatus();
        Console.WriteLine("");

        Disk winner = _board.Result();
        Console.WriteLine(
            winner == Disk.Empty
                ? "The game ended in a tie...\n"
                : $"The winner is {winner.DiskString()}!\n"
        );
    }

    /// <summary>Print current board and player info.</summary>
    private void PrintStatus() {
        Console.WriteLine(_playerBlack);
        Console.WriteLine($"{_playerWhite}\n");
        Console.WriteLine(_board);
    }

    /// <summary>Ask a question with two options, and return bool from user answer.</summary>
    private static bool GetAnswer(string question, string yes = "y", string no = "n") {
        Console.Write($"{question} ({yes}/{no})? ");
        string? answer = Console.ReadLine();
        return answer != null
            && string.Equals(answer.Trim(), yes, StringComparison.OrdinalIgnoreCase);
    }

    /// <summary>Ask and return the desired board size.</summary>
    public static int GetBoardSize() {
        Console.Write($"Choose board size (default is {Settings.DefaultBoardSize}): ");
        string? input = Console.ReadLine();
        if (input != null && int.TryParse(input.Trim(), out int boardSize)) {
            if (boardSize is < Settings.MinBoardSize or > Settings.MaxBoardSize) {
                ColorPrint.PrintYellow(
                    $"Limiting board size to valid range {Settings.MinBoardSize}..{Settings.MaxBoardSize}"
                );
            }
            return Math.Clamp(boardSize, Settings.MinBoardSize, Settings.MaxBoardSize);
        }
        ColorPrint.PrintWarn($"Invalid size, defaulting to {Settings.DefaultBoardSize}...");
        return Settings.DefaultBoardSize;
    }
}
