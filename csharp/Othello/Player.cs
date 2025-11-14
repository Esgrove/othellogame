//==========================================================
// Class Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2025
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Threading;

namespace Othello
{
    /// Defines one player that can be either human or computer controlled.
    internal sealed class Player(Disk color, PlayerSettings settings)
    {
        public bool CanPlay = true;
        private bool _isHuman = true;
        private int _roundsPlayed;
        private readonly Disk _disk = color;
        private readonly Random _random = new();
        private readonly PlayerSettings _settings = settings;

        /// Shorthand to initialize a new player for black disks.
        public static Player Black(PlayerSettings settings)
        {
            return new Player(Disk.Black, settings);
        }

        /// Shorthand to initialize a new player for white disks.
        public static Player White(PlayerSettings settings)
        {
            return new Player(Disk.White, settings);
        }

#nullable enable
        /// Play one round as this player.
        public string? PlayOneMove(Board board)
        {
            if (!_settings.CheckMode)
            {
                Console.WriteLine($"Turn: {_disk.Name()}");
            }
            List<Move>? moves = board.PossibleMoves(_disk);
            if (moves.Count != 0)
            {
                CanPlay = true;
                if (_isHuman && _settings is { ShowHelpers: true, CheckMode: false })
                {
                    board.PrintPossibleMoves(moves);
                }
                Move chosenMove = _isHuman ? GetHumanMove(moves) : GetComputerMove(moves);
                board.PlaceDisc(chosenMove);
                if (!_settings.CheckMode)
                {
                    board.PrintScore();
                }
                ++_roundsPlayed;
                if (!_settings.TestMode)
                {
                    Thread.Sleep(1000);
                }
                return chosenMove.LogEntry();
            }

            CanPlay = false;
            if (!_settings.CheckMode)
            {
                ColorPrint.WriteLine("  No moves available...", Color.Yellow);
            }
            return null;
        }

#nullable disable

        /// Set player to be controlled by human or computer.
        public void SetHuman(bool isHuman)
        {
            _isHuman = isHuman;
        }

        /// Reset player status for a new game.
        public void Reset()
        {
            CanPlay = true;
            _roundsPlayed = 0;
        }

        /// Return move chosen by computer.
        private Move GetComputerMove(List<Move> moves)
        {
            if (!_settings.CheckMode)
            {
                Console.WriteLine("  Computer plays...");
            }
            Move chosenMove;
            if (_settings.TestMode)
            {
                chosenMove = moves[0];
            }
            else
            {
                // Wait a bit and pick a random move
                Thread.Sleep(_random.Next(1000, 2000));
                chosenMove = moves[_random.Next(moves.Count)];
            }
            if (!_settings.CheckMode)
            {
                Console.WriteLine($"  {chosenMove.Square} -> {chosenMove.Value}");
            }
            return chosenMove;
        }

        /// Return move chosen by a human player.
        private Move GetHumanMove(List<Move> moves)
        {
            while (true)
            {
                var square = GetSquare();
                // check if given square is one of the possible moves
                if (moves.Exists(x => square.Equals(x.Square)))
                {
                    return moves.Find(x => square.Equals(x.Square));
                }
                ColorPrint.Error($"  Can't place a {_disk.Name()} disk in square {square}!");
            }
        }

        /// Ask human player for square coordinates.
        private static Square GetSquare()
        {
            while (true)
            {
                try
                {
                    Console.Write("  Give disk position (x,y): ");
                    string coords = Console.ReadLine();
                    if (string.IsNullOrEmpty(coords) || coords.Length != 3 || coords[1] != ',')
                    {
                        throw new FormatException("Invalid coordinates");
                    }
                    int x = int.Parse(coords[..1]);
                    int y = int.Parse(coords[2..3]);
                    return new Square(x, y);
                }
                catch (FormatException)
                {
                    ColorPrint.Error("  Give coordinates in the form 'x,y'");
                }
            }
        }

        /// Return player type description string.
        private string TypeString()
        {
            return _isHuman ? "Human   " : "Computer";
        }

        public override string ToString()
        {
            return $"{_disk.Name()} | {TypeString()} | Moves: {_roundsPlayed}";
        }
    }
}
