using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using Pastel;

namespace Othello
{
    /// Handles game board state and logic.
    internal class Board
    {
        private readonly List<Disk> _board;
        private readonly List<Square> _emptySquares;
        private readonly List<int> _indices;
        private readonly int _size;

        private static readonly int[][] StepDirections =
        {
            new [] {-1,-1},
            new [] {-1, 0},
            new [] {-1, 1},
            new [] { 0,-1},
            new [] { 0, 1},
            new [] { 1,-1},
            new [] { 1, 0},
            new [] { 1, 1}
        };

        public Board(int size) {
            _size = size;
            var numSquares = _size * _size;
            // init game board with empty disks
            _board = Enumerable.Repeat(Disk.Empty, numSquares).ToList();

            // set starting positions
            var row = _size % 2 == 0 ? (_size - 1) / 2 : (_size - 1) / 2 - 1;
            var col = _size / 2;
            _board[row * _size + row] = Disk.Black;
            _board[row * _size + col] = Disk.White;
            _board[col * _size + row] = Disk.White;
            _board[col * _size + col] = Disk.Black;

            // index list (0...size) to avoid repeating same range in for loops
            _indices = Enumerable.Range(0, _size).ToList();

            // keep track of empty squares on board to avoid checking already filled positions
            _emptySquares = new List<Square>(numSquares);
            foreach (var y in _indices) {
                foreach (var x in _indices.Where(x => GetSquare(x, y) == Disk.Empty)) {
                    _emptySquares.Add(new Square(x, y));
                }
            }
        }

        /// Return true if board contains empty squares
        public bool CanPlay() {
            return _emptySquares.Any();
        }

        /// Update board for given disk placement.
        public void PlaceDisc(Move move) {
            var start = move.Square;
            if (GetSquare(start.X, start.Y) != Disk.Empty) {
                throw new ArgumentException($"Trying to place disk to an occupied square {start}!");
            }
            SetSquare(start.X, start.Y, move.Disk);
            _emptySquares.Remove(start);
            foreach (var dir in move.Directions) {
                var pos = start + dir;
                while (GetSquare(pos.X, pos.Y) == move.Disk.OtherDisk()) {
                    SetSquare(pos.X, pos.Y, move.Disk);
                    pos += dir;
                }
            }
        }

        /// Returns a list of possible moves for given player.
        public List<Move> PossibleMoves(Disk color) {
            var moves = new List<Move>();
            var other = color.OtherDisk();
            foreach (var square in _emptySquares) {
                var value = 0;
                var directions = new List<Square>();
                foreach (var dir in StepDirections) {
                    var step = new Square(dir[0], dir[1]);
                    var pos = square + step;
                    // next square in this directions needs to be opponents disk
                    if (GetSquare(pos.X, pos.Y) != other) {
                        continue;
                    }
                    var steps = 0;
                    // keep stepping forward while opponents disks are found
                    while (GetSquare(pos.X, pos.Y) == other) {
                        ++steps;
                        pos += step;
                    }
                    // valid move if a line of opponents disks ends in own disk
                    if (GetSquare(pos.X, pos.Y) != color) {
                        continue;
                    }
                    value += steps;
                    directions.Add(step);
                }
                if (value > 0) {
                    moves.Add(new Move(square, value, color, directions));
                }
            }
            if (moves.Any()) {
                moves.Sort(delegate(Move left, Move right) {
                    var value = left.Value.CompareTo(right.Value);
                    return value == 0 ? left.Square.X.CompareTo(right.Square.X) : value;
                });
            }
            return moves;
        }

        /// Format game board to string
        public void Print() {
            Console.Write(" ");
            foreach (var i in _indices) {
                Console.Write(" " + i);
            }
            Console.Write("\n");
            foreach (var y in _indices) {
                Console.Write(y);
                foreach (var disk in _indices.Select(x => _board[y * _size + x])) {
                    ColorPrint.Write(" " + disk.BoardChar(), disk.Color());
                }
                Console.Write("\n");
            }
        }

        /// Print available move coordinates and resulting points gained.
        public void PrintPossibleMoves(IReadOnlyCollection<Move> moves) {
            ColorPrint.Write($"  Possible moves ({moves.Count}):\n", ConsoleColor.Yellow);
            foreach (var move in moves) {
                Console.WriteLine($"  {move}");
            }
            // TODO: add board print
        }

        /// Print current score for both players.
        public void PrintScore() {
            var (black, white) = PlayerScores();
            Console.WriteLine("");
            Print();
            Console.Write("Score: ");
            ColorPrint.Write($"{black}", Disk.Black.Color());
            Console.Write(" | ");
            ColorPrint.Write($"{white}\n\n", Disk.White.Color());
        }

        /// Calculates the final score and returns the winning player.
        public Disk Result() {
            var sum = Score();
            if (sum == 0) {
                return Disk.Empty;
            }
            return sum > 0 ? Disk.White : Disk.Black;
        }

        /// Check that the given coordinates are inside the board
        private bool CheckCoordinates(int x, int y) {
            return 0 <= x && x < _size && 0 <= y && y < _size;
        }

        /// Count and return the number of black and white disks.
        private (int, int) PlayerScores() {
            var black = 0;
            var white = 0;
            foreach (var disk in _board) {
                switch (disk) {
                    case Disk.White:
                        ++white;
                        break;
                    case Disk.Black:
                        ++black;
                        break;
                    case Disk.Empty:
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
            return (black, white);
        }

        /// Returns the total score (positive means more white disks and negative means more black disks).
        private int Score() {
            return _board.Sum(x => Convert.ToInt32(x));
        }

        /// Sets the given square to given value.
        private void SetSquare(int x, int y, Disk disk) {
            if (!CheckCoordinates(x, y)) {
                throw new ArgumentException($"Invalid coordinates ({x},{y})!");
            }
            _board[y * _size + x] = disk;
        }

        /// Returns the state of the board (empty, white, black) at the given coordinates.
        private Disk? GetSquare(int x, int y) {
            if (!CheckCoordinates(x, y)) {
                return null;
            }
            return _board[y * _size + x];
        }
    }
}
