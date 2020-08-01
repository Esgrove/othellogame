using System;
using System.Collections.Generic;
using System.Threading;

namespace Othello
{
    internal class Player
    {
        public Player(Disk color) {
            _disk = color;
            _human = true;
            _random = new Random();
            _canPlay = true;
            _showHelpers = true;
        }

        public bool CanPlay() {
            return _canPlay;
        }

        public void PlayOneMove(Board board) {
            Console.Write("Turn: ");
            ColorPrint.Write(DiskStringUpper() + "\n", _disk.DiskColor());
            var moves = board.PossibleMoves(_disk);
            if (moves.Count > 0) {
                _canPlay = true;
                if (_showHelpers && _human) {
                    PrintPossibleMoves(moves);
                }
                var move = _human ? getHumanMove(moves) : getComputerMove(moves);
                board.PlaceDisc(move);
                ++_roundsPlayed;
                board.PrintScore();
            } else {
                Console.WriteLine("  No moves available...");
                _canPlay = false;
            }
        }

        public void Print() {
            ColorPrint.Write(_disk.ToString().ToUpper(), _disk.DiskColor());
            Console.WriteLine($" | {TypeString()} | moves: {_roundsPlayed}");
        }

        public void SetPlayerType(bool isHuman) {
            _human = isHuman;
        }

        public override string ToString() {
            return $"{_disk.ToString().ToUpper()} | {TypeString()} | moves: {_roundsPlayed}";
        }

        /// Return move chosen by computer.
        private Move getComputerMove(List<Move> moves) {
            Console.WriteLine("  Computer plays...");
            // wait a bit and pick a random move
            Thread.Sleep(_random.Next(1000, 2000));
            var move = moves[_random.Next(moves.Count)];
            Console.WriteLine($"  -> {move.Square}");
            return move;
        }

        /// Return move chosen by computer.
        private Move getHumanMove(List<Move> moves) {
            while (true) {
                var square = getSquare();
                if (moves.Exists(x => x.Square.Equals(square))) {
                    var move = moves.Find(x => x.Square.Equals(square));
                    return move;
                } else {
                    ColorPrint.Error($"can't place a {_disk.Name()} disk in square {square}!\n");
                }
            }
        }

        /// Ask human player for square coordinates.
        private Square getSquare() {
            while (true) {
                try {
                    Console.Write("  Give disk position (x,y): ");
                    var coords = Console.ReadLine();
                    if (string.IsNullOrEmpty(coords) || coords.Length != 3 || coords[1] != ',') {
                        throw new FormatException();
                    }
                    var x = Convert.ToInt32(coords[0].ToString());
                    var y = Convert.ToInt32(coords[2].ToString());
                    return new Square(x, y);
                } catch (FormatException) {
                    ColorPrint.Error("give coordinates in the form (x,y)!\n");
                }
            }
        }

        private static void PrintPossibleMoves(IReadOnlyCollection<Move> moves) {
            ColorPrint.Write($"  Possible plays ({moves.Count}):\n", ConsoleColor.Yellow);
            foreach (var move in moves) {
                Console.WriteLine($"  {move}");
            }
        }

        private string DiskStringUpper() {
            return _disk.ToString().ToUpper();
        }

        private string TypeString() {
            return _human ? "Human   " : "Computer";
        }

        private bool _canPlay;
        private bool _human;
        private int _roundsPlayed;
        private readonly bool _showHelpers;
        private readonly Disk _disk;
        private readonly Random _random;
    }
}
