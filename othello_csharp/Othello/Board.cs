using System;
using System.Collections.Generic;
using System.Linq;

namespace Othello
{
    internal class Board
    {
        public Board(int dimension)
        {
            _dim = dimension;
            var size = _dim * _dim;
            _board = Enumerable.Repeat(Disk.Empty, size).ToList();
            // init board with starting positions
            var row = _dim % 2 == 0 ? (_dim - 1) / 2 : (_dim - 1) / 2 - 1;
            var col = _dim / 2;
            _board[row * _dim + row] = Disk.Black;
            _board[row * _dim + col] = Disk.White;
            _board[col * _dim + row] = Disk.White;
            _board[col * _dim + col] = Disk.Black;
        }

        private bool CanPlaceToSquare(int x, int y, Disk color)
        {
            if (Square(x, y) != Disk.Empty) return false;
            var other = color.OtherDisk();
            foreach (var dir in Directions) {
                var tx = x + dir[0];
                var ty = y + dir[1];
                if (Square(tx, ty) != other) continue;
                while (Square(tx, ty) == other) {
                    tx += dir[0];
                    ty += dir[1];
                }
                if (Square(tx, ty) == color) {
                    return true;
                }
            }
            return false;
        }

        private bool CheckCoordinates(int x, int y)
        {
            return x >= 0 && x < _dim && y >= 0 && y < _dim;
        }

        public List<Move> GetPossibleGetMoves(Disk color)
        {
            var moves = new List<Move>();
            var other = color.OtherDisk();
            for (var y = 0; y < _dim; ++y) {
                for (var x = 0; x < _dim; ++x) {
                    if (Square(x, y) != Disk.Empty) continue;
                    var value = 0;
                    foreach (var dir in Directions) {
                        var tx = x + dir[0];
                        var ty = y + dir[1];
                        if (Square(tx, ty) != other) continue;
                        var steps = 0;
                        while (Square(tx, ty) == other) {
                            tx += dir[0];
                            ty += dir[1];
                            ++steps;
                        }
                        if (Square(tx, ty) == color) {
                            value += steps;
                        }
                    }
                    if (value > 0) {
                        moves.Add(new Move(x, y, value));
                    }
                }
            }
            return moves;
        }

        public bool PlaceDisc(int x, int y, Disk color)
        {
            if (!CanPlaceToSquare(x, y, color)) return false;
            var other = color.OtherDisk();
            foreach (var dir in Directions) {
                var tx = x + dir[0];
                var ty = y + dir[1];
                if (Square(tx, ty) != other) continue;
                while (Square(tx, ty) == other) {
                    tx += dir[0];
                    ty += dir[1];
                }
                if (Square(tx, ty) != color) continue;
                while ( tx != x || ty != y ) {
                    tx -= dir[0];
                    ty -= dir[1];
                    SetSquare(tx, ty, color);
                }
            }
            return true;
        }

        public void PrintScore()
        {
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
                }
            }
            Console.Write("Score: ");
            ColorPrint.Write(black.ToString(), Disk.Black.DiskColor());
            Console.Write(" | ");
            ColorPrint.Write(white + "\n\n", Disk.White.DiskColor());
        }

        public Disk Result()
        {
            var sum = Score();
            if (sum == 0) {
                return Disk.Empty;
            }
            return sum > 0 ? Disk.White : Disk.Black;
        }

        private int Score()
        {
            return _board.Sum(x => Convert.ToInt32(x));
        }

        private void SetSquare(int x, int y, Disk disk)
        {
            if (!CheckCoordinates(x, y)) {
                throw new ArgumentException($"Coordinates '{x},{y}' are not valid!");
            }
            _board[y * _dim + x] = disk;
        }

        private Disk Square(int x, int y)
        {
            return CheckCoordinates(x, y) ? _board[y * _dim + x] : Disk.Invalid;
        }

        public void Print()
        {
            Console.Write(" ");
            for (var i = 0; i < _dim; ++i) {
                Console.Write(" " + i);
            }
            Console.Write("\n");
            for (var y = 0; y < _dim; ++y) {
                Console.Write(y);
                for (var x = 0; x < _dim; ++x) {
                    var disk = _board[y * _dim + x];
                    ColorPrint.Write(" " + disk.GetBoardChar(), disk.DiskColor());
                }
                Console.Write("\n");
            }
            Console.Write("\n");
        }

        private static readonly int[][] Directions =
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

        private readonly List<Disk> _board;
        private readonly int _dim;
    }
}
