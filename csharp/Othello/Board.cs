//==========================================================
// Class Board
// Defines the game board
// Akseli Lukkarila
// 2019-2024
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Othello
{
    /// Handles game board state and logic.
    internal class Board
    {
        private readonly List<Disk> _board;
        private readonly List<Square> _emptySquares;
        private readonly List<int> _indices;
        private readonly int _size;

        private static readonly Step[] StepDirections =
        {
            new(-1, -1),
            new(-1, 0),
            new(-1, 1),
            new(0, -1),
            new(0, 1),
            new(1, -1),
            new(1, 0),
            new(1, 1)
        };

        public Board(int size)
        {
            _size = size;
            var numSquares = _size * _size;
            // Init game board with empty disks
            _board = Enumerable.Repeat(Disk.Empty, numSquares).ToList();

            // Set starting positions
            var row = _size % 2 == 0 ? (_size - 1) / 2 : (_size - 1) / 2 - 1;
            var col = _size / 2;
            _board[row * _size + row] = Disk.White;
            _board[row * _size + col] = Disk.Black;
            _board[col * _size + row] = Disk.Black;
            _board[col * _size + col] = Disk.White;

            // Index list (0...size) to avoid repeating same range in for loops
            _indices = Enumerable.Range(0, _size).ToList();

            // Keep track of empty squares on board to avoid checking already filled positions
            _emptySquares = new List<Square>(numSquares);
            foreach (
                var square in _indices
                    .SelectMany(y => _indices, (y, x) => new Square(x, y))
                    .Where(square => GetSquare(square) == Disk.Empty)
            )
            {
                _emptySquares.Add(square);
            }
        }

        /// Return true if board contains empty squares.
        /// -> still possible to make a move.
        public bool CanPlay()
        {
            return _emptySquares.Count != 0;
        }

        /// Update board for given disk placement.
        public void PlaceDisc(Move move)
        {
            var start = move.Square;
            if (GetSquare(start) != Disk.Empty)
            {
                throw new ArgumentException($"Trying to place disk to an occupied square {start}!");
            }
            SetSquare(start, move.Disk);
            _emptySquares.Remove(start);
            foreach (var dir in move.Directions)
            {
                var pos = start + dir;
                while (GetSquare(pos) == move.Disk.Opponent())
                {
                    SetSquare(pos, move.Disk);
                    pos += dir;
                }
            }
        }

        /// Returns a list of possible moves for the given player.
        public List<Move> PossibleMoves(Disk color)
        {
            var moves = new List<Move>();
            var other = color.Opponent();
            foreach (var square in _emptySquares)
            {
                var value = 0;
                var directions = new List<Step>();
                foreach (var dir in StepDirections)
                {
                    var step = new Square(dir.X, dir.Y);
                    var pos = square + step;
                    // next square in this directions needs to be opponents disk
                    if (GetSquare(pos) != other)
                    {
                        continue;
                    }
                    var steps = 0;
                    // keep stepping forward while opponents disks are found
                    while (GetSquare(pos) == other)
                    {
                        ++steps;
                        pos += step;
                    }
                    // valid move if a line of opponents disks ends in own disk
                    if (GetSquare(pos) != color)
                    {
                        continue;
                    }
                    value += steps;
                    directions.Add(dir);
                }
                if (value > 0)
                {
                    moves.Add(new Move(square, value, color, directions));
                }
            }
            if (moves.Count != 0)
            {
                moves.Sort();
            }
            return moves;
        }

        /// Print board with available move coordinates and the resulting points gained.
        public void PrintPossibleMoves(IReadOnlyCollection<Move> moves)
        {
            ColorPrint.WriteLine($"  Possible moves ({moves.Count}):", Color.Yellow);
            // Convert board from Disk enums to strings
            var formattedBoard = new List<string>(_board.Count);
            formattedBoard.AddRange(_board.Select(disk => disk.BoardChar()));
            foreach (var move in moves)
            {
                Console.WriteLine($"  {move}");
                var (x, y) = move.Square;
                formattedBoard[y * _size + x] = ColorPrint.Get(move.Value, Color.Yellow);
            }
            // Print board with move positions
            Console.Write("   ");
            foreach (var i in _indices)
            {
                Console.Write($" {i}");
            }
            foreach (var y in _indices)
            {
                Console.Write($"\n  {y}");
                foreach (var x in _indices)
                {
                    Console.Write($" {formattedBoard[y * _size + x]}");
                }
            }
            Console.WriteLine("");
        }

        /// Print current score for both players.
        public void PrintScore()
        {
            var (black, white) = PlayerScores();
            Console.WriteLine($"\n{this}");
            Console.WriteLine(
                $"Score: {ColorPrint.Get(black, Disk.Black.DiskColor())} | "
                    + $"{ColorPrint.Get(white, Disk.White.DiskColor())}"
            );
        }

        /// Get board status string for game log.
        public string LogEntry()
        {
            return _board.Aggregate(
                new StringBuilder(),
                (accumulator, disk) => accumulator.Append(disk.BoardChar(false)),
                accumulator => accumulator.ToString()
            );
        }

        /// Calculates the final score and returns the winning player.
        public Disk Result()
        {
            var sum = Score();
            if (sum == 0)
            {
                return Disk.Empty;
            }
            return sum > 0 ? Disk.White : Disk.Black;
        }

        /// Check that the given coordinates are inside the board.
        private bool CheckCoordinates(int x, int y)
        {
            return 0 <= x && x < _size && 0 <= y && y < _size;
        }

        /// Count and return the number of black and white disks.
        private (int, int) PlayerScores()
        {
            var black = 0;
            var white = 0;
            foreach (var disk in _board)
            {
                switch (disk)
                {
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
        private int Score()
        {
            return _board.Sum(x => Convert.ToInt32(x));
        }

        /// Returns the state of the board (empty, white, black) at the given coordinates.
        private Disk? GetSquare(Square square)
        {
            var (x, y) = square;
            if (!CheckCoordinates(x, y))
            {
                return null;
            }
            return _board[y * _size + x];
        }

        /// Sets the given square to given value.
        private void SetSquare(Square square, Disk disk)
        {
            var (x, y) = square;
            if (!CheckCoordinates(x, y))
            {
                throw new ArgumentException($"Invalid coordinates ({x},{y})!");
            }
            _board[y * _size + x] = disk;
        }

        /// Format game board to string
        public override string ToString()
        {
            var text = _indices.Aggregate(" ", (current, i) => current + $" {i}");
            foreach (var y in _indices)
            {
                text += $"\n{y}";
                text = _indices
                    .Select(x => _board[y * _size + x])
                    .Aggregate(text, (current, disk) => current + $" {disk.BoardChar()}");
            }
            return text;
        }
    }
}
