//==========================================================
// Class Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2023
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
        private bool _human;
        private int _roundsPlayed;
        private readonly Disk _disk;
        private readonly Random _random;
        private readonly bool _showHelpers;

        public Player(Disk color)
        {
            _disk = color;
            _human = true;
            _random = new Random();
            canPlay = true;
            _showHelpers = true;
        }

        /// Play one round as this player.
        public void PlayOneMove(Board board)
        {
            Console.WriteLine($"Turn: {_disk.Name()}");
            var moves = board.PossibleMoves(_disk);
            if (moves.Any())
            {
                canPlay = true;
                if (_human && _showHelpers)
                {
                    board.PrintMoves(moves);
                }
                var chosenMove = _human ? GetHumanMove(moves) : GetComputerMove(moves);
                board.PlaceDisc(chosenMove);
                board.PrintScore();
                ++_roundsPlayed;
                Thread.Sleep(1000);
            }
            else
            {
                canPlay = false;
                ColorPrint.WriteLine("  No moves available...", Color.Yellow);
            }
        }

        /// Set player to be controlled by human or computer.
        public void SetHuman(bool isHuman)
        {
            _human = isHuman;
        }

        public override string ToString()
        {
            return $"{_disk.Name()} | {TypeString()} | Moves: {_roundsPlayed}";
        }

        public void reset()
        {
            _roundsPlayed = 0;
            canPlay = true;
        }

        /// Return move chosen by computer.
        private Move GetComputerMove(IReadOnlyList<Move> moves)
        {
            Console.WriteLine("  Computer plays...");
            // Wait a bit and pick a random move
            Thread.Sleep(_random.Next(1000, 2000));
            var chosenMove = moves[_random.Next(moves.Count)];
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
                ColorPrint.Error($"can't place a {_disk.Name()} disk in square {square}!", 2);
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
                        throw new FormatException();
                    }
                    var x = int.Parse(coords.Substring(0, 1));
                    var y = int.Parse(coords.Substring(2, 1));
                    return new Square(x, y);
                }
                catch (FormatException)
                {
                    ColorPrint.Error("give coordinates in the form (x,y)!", 2);
                }
            }
        }

        private string TypeString()
        {
            return _human ? "Human   " : "Computer";
        }
    }
}
