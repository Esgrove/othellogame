//==========================================================
// Class Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2024
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Threading;

namespace Othello
{
    /// Defines one player (human or computer).
    internal class Player
    {
        public bool canPlay;
        private bool _isHuman;
        private int _roundsPlayed;
        private readonly Disk _disk;
        private readonly Random _random;
        private readonly PlayerSettings _settings;

        public Player(Disk color, PlayerSettings settings)
        {
            canPlay = true;
            _disk = color;
            _isHuman = true;
            _random = new Random();
            _settings = settings;
        }

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
            Console.WriteLine($"Turn: {_disk.Name()}");
            var moves = board.PossibleMoves(_disk);
            if (moves.Count != 0)
            {
                canPlay = true;
                if (_isHuman && _settings.ShowHelpers)
                {
                    board.printPossibleMoves(moves);
                }
                var chosenMove = _isHuman ? GetHumanMove(moves) : GetComputerMove(moves);
                board.PlaceDisc(chosenMove);
                board.PrintScore();
                ++_roundsPlayed;
                if (!_settings.TestMode)
                {
                    Thread.Sleep(1000);
                }
                return chosenMove.ToLogEntry();
            }

            canPlay = false;
            ColorPrint.WriteLine("  No moves available...", Color.Yellow);
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
            canPlay = true;
            _roundsPlayed = 0;
        }

        /// Return move chosen by computer.
        private Move GetComputerMove(IReadOnlyList<Move> moves)
        {
            Console.WriteLine("  Computer plays...");
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
            Console.WriteLine($"  {chosenMove.Square} -> {chosenMove.Value}");
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
                    var coords = Console.ReadLine();
                    if (string.IsNullOrEmpty(coords) || coords.Length != 3 || coords[1] != ',')
                    {
                        throw new FormatException("Invalid coordinates");
                    }
                    var x = int.Parse(coords[0..1]);
                    var y = int.Parse(coords[2..3]);
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
