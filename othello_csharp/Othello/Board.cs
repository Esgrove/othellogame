using System;
using System.Collections.Generic;
using System.Linq;

namespace Othello
{
    internal class Board
    {
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

            // keep track of empty squares on board to avoid checking already filled positions
            emptySquares = new List<Square>(numSquares);
            foreach (var y in Enumerable.Range(0, _size)) {
                foreach (var x in Enumerable.Range(0, _size)) {
                    if (Square(x, y) == Disk.Empty) {
                        emptySquares.Add(new Square(x, y));
                    }
                }
            }
        }

        /// Return true if board contains empty squares
        public bool CanPlay() {
            return emptySquares.Any();
        }

        /// Check that the given coordinates are inside the board
        private bool CheckCoordinates(int x, int y) {
            return x >= 0 && x < _size && y >= 0 && y < _size;
        }

        /// Returns a list of possible moves for given player.
        public List<Move> PossibleMoves(Disk color) {
            var moves = new List<Move>();
            var other = color.OtherDisk();
            foreach (var square in emptySquares) {
                var value = 0;
                var directions = new List<Square>();
                foreach (var dir in DIRECTIONS) {
                    var tx = square.X + dir[0];
                    var ty = square.Y + dir[1];
                    // next square in this directions needs to be opponents disk
                    if (Square(tx, ty) != other) {
                        continue;
                    }
                    var steps = 0;
                    // keep stepping forward while opponents disks are found
                    while (Square(tx, ty) == other) {
                        ++steps;
                        tx += dir[0];
                        ty += dir[1];
                    }
                    // valid move if a line of opponents disks ends in own disk
                    if (Square(tx, ty) == color) {
                        value += steps;
                        directions.Add(new Square(dir[0], dir[1]));
                    }
                }
                if (value > 0) {
                    moves.Add(new Move(square, value, color, directions));
                }
            }
            if (moves.Any()) {
                // sort moves in descending order according to value
                moves.Sort((x, y) => y.Value.CompareTo(x.Value));
            }
            return moves;
        }

        /// Update board for given disk placement.
        public void PlaceDisc(Move move) {
            var start = move.Square;
            SetSquare(start.X, start.Y, move.Disk);
            emptySquares.Remove(start);
            foreach (var step in move.Directions) {
                var pos = start + step;
                while (Square(pos.X, pos.Y) == move.Disk.OtherDisk()) {
                    SetSquare(pos.X, pos.Y, move.Disk);
                    pos += step;
                }
            }
        }

        /// Print current score for both players.
        public void PrintScore() {
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
            Console.Write("");
            Print();
            Console.Write("Score: ");
            ColorPrint.Write(black.ToString(), Disk.Black.DiskColor());
            Console.Write(" | ");
            ColorPrint.Write(white + "\n\n", Disk.White.DiskColor());
        }

        /// Calculates the final score and returns the winner color.
        public Disk Result() {
            var sum = Score();
            if (sum == 0) {
                return Disk.Empty;
            }
            return sum > 0 ? Disk.White : Disk.Black;
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
        private Disk Square(int x, int y) {
            return CheckCoordinates(x, y) ? _board[y * _size + x] : Disk.ERROR;
        }

        /// Format game board to string
        public void Print() {
            Console.Write(" ");
            for (var i = 0; i < _size; ++i) {
                Console.Write(" " + i);
            }
            Console.Write("\n");
            for (var y = 0; y < _size; ++y) {
                Console.Write(y);
                for (var x = 0; x < _size; ++x) {
                    var disk = _board[y * _size + x];
                    ColorPrint.Write(" " + disk.GetBoardChar(), disk.DiskColor());
                }
                Console.Write("\n");
            }
            Console.Write("\n");
        }

        private static readonly int[][] DIRECTIONS =
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
        private readonly int _size;
        private readonly List<Square> emptySquares;
    }
}
