//==========================================================
// Class Board
// Defines the game board
// Akseli Lukkarila
// 2019-2026
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Othello;

/// <summary>Handles game board state and logic.</summary>
internal sealed class Board {
    private const int Up = 1;
    private const int Down = -1;
    private const int Left = -1;
    private const int Right = 1;
    private const int Still = 0;

    /// <summary>All possible step directions for a square on the board.</summary>
    private static readonly Step[] StepDirections =
    [
        new(Down, Left),
        new(Down, Right),
        new(Down, Still),
        new(Still, Left),
        new(Still, Right),
        new(Up, Left),
        new(Up, Right),
        new(Up, Still),
    ];

    private readonly List<Disk> _board;
    private readonly List<Square> _emptySquares;
    private readonly List<int> _indices;
    internal readonly int Size;

    /// <summary>Initialize a new board for the given board size.</summary>
    public Board(int size) {
        _board = InitBoard(size);

        // Index list (0...size) to avoid repeating same range in loops.
        _indices = [.. Enumerable.Range(0, size)];

        // Keep track of empty squares on board to avoid checking already filled positions.
        _emptySquares = InitEmptySquares(size, _board);

        Size = size;
    }

    /// <summary>Return true if board contains empty squares.</summary>
    public bool CanPlay() {
        return _emptySquares.Count != 0;
    }

    /// <summary>Update board for given disk placement.</summary>
    public void PlaceDisk(Move chosenMove) {
        Square start = chosenMove.Square;
        Disk? oldDisk =
            GetSquare(start)
            ?? throw new ArgumentException($"Invalid coordinates: {start}");
        if (oldDisk != Disk.Empty) {
            throw new ArgumentException($"Trying to place disk to an occupied square: {start}!");
        }
        SetSquare(start, chosenMove.Disk);
        _emptySquares.Remove(start);
        foreach (Square square in chosenMove.AffectedSquares()) {
            SetSquare(square, chosenMove.Disk);
        }
    }

    /// <summary>Returns a list of possible moves for the given player.</summary>
    public List<Move> PossibleMoves(Disk disk) {
        List<Move> moves = [];
        Disk opposingDisk = disk.Opponent();
        foreach (Square square in _emptySquares) {
            int value = 0;
            List<Direction> directions = [];
            foreach (Step step in StepDirections) {
                Square pos = square + step;
                // Next square in this direction needs to be the opposing disk
                if (GetSquare(pos) != opposingDisk) {
                    continue;
                }
                int numSteps = 0;
                // Keep stepping over opponents disks
                while (GetSquare(pos) == opposingDisk) {
                    ++numSteps;
                    pos += step;
                }
                // Valid move only if a line of opposing disks ends with own disk
                if (GetSquare(pos) == disk) {
                    directions.Add(new Direction(step, numSteps));
                    value += numSteps;
                }
            }
            if (value > 0) {
                moves.Add(new Move(square, disk, value, directions));
            }
        }
        moves.Sort();
        return moves;
    }

    /// <summary>Print board with available move coordinates and the resulting points gained.</summary>
    public void PrintPossibleMoves(IReadOnlyCollection<Move> moves) {
        ColorPrint.PrintYellow($"  Possible moves ({moves.Count}):");
        // Convert board from Disk enums to strings
        List<string> formattedBoard = [.. _board.Select(disk => disk.BoardCharWithColor())];

        // Add possible moves to board
        foreach (Move possibleMove in moves) {
            int index = SquareIndex(possibleMove.Square);
            formattedBoard[index] = ColorPrint.Get(possibleMove.Value, Color.Yellow);
            Console.WriteLine($"  {possibleMove}");
        }
        // Print board with move positions
        Console.Write("    ");
        foreach (int i in _indices) {
            Console.Write($" {ColorPrint.Bold(i)}");
        }
        foreach (int y in _indices) {
            Console.Write($"\n  {ColorPrint.Bold(y)}");
            foreach (int x in _indices) {
                Console.Write($" {formattedBoard[y * Size + x]}");
            }
        }
        Console.WriteLine();
    }

    /// <summary>Print current score for both players.</summary>
    public void PrintScore() {
        (int black, int white) = PlayerScores();
        Console.WriteLine($"\n{this}");
        Console.WriteLine(
            $"Score: {ColorPrint.Get(black, Disk.Black.DiskColor())} | "
                + $"{ColorPrint.Get(white, Disk.White.DiskColor())}"
        );
    }

    /// <summary>Returns the winning disk colour. Empty indicates a draw.</summary>
    public Disk Result() {
        int totalScore = Score();
        if (totalScore == 0) {
            return Disk.Empty;
        }
        return totalScore > 0 ? Disk.White : Disk.Black;
    }

    /// <summary>Get board status string for game log.</summary>
    public string LogEntry() {
        return _board.Aggregate(
            new StringBuilder(),
            (accumulator, disk) => accumulator.Append(disk.BoardChar()),
            accumulator => accumulator.ToString()
        );
    }

    /// <summary>Check that the given coordinates are valid (inside the board).</summary>
    private bool CheckCoordinates(int x, int y) {
        return 0 <= x && x < Size && 0 <= y && y < Size;
    }

    /// <summary>Check that the given square is valid (inside the board).</summary>
    private bool CheckSquare(Square square) {
        return CheckCoordinates(square.X, square.Y);
    }

    /// <summary>Returns the state of the board (empty, white, black) at the given square.</summary>
    internal Disk? GetSquare(Square square) {
        if (CheckSquare(square)) {
            return _board[SquareIndex(square)];
        }
        return null;
    }

    /// <summary>Map square to board index.</summary>
    private int SquareIndex(Square square) {
        return square.Y * Size + square.X;
    }

    /// <summary>Count and return the number of black and white disks.</summary>
    internal (int black, int white) PlayerScores() {
        int black = 0;
        int white = 0;
        foreach (Disk disk in _board) {
            switch (disk) {
                case Disk.Black:
                    ++black;
                    break;
                case Disk.White:
                    ++white;
                    break;
                case Disk.Empty:
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
        return (black, white);
    }

    /// <summary>
    /// Returns the total score.
    /// Positive value means more white disks and negative means more black disks.
    /// </summary>
    internal int Score() {
        return _board.Sum(static disk => (int)disk);
    }

    /// <summary>Sets the given square to the given value.</summary>
    private void SetSquare(Square square, Disk disk) {
        if (!CheckSquare(square)) {
            throw new ArgumentException($"Invalid coordinates: {square}");
        }
        _board[SquareIndex(square)] = disk;
    }

    /// <summary>Initialize game board with starting disk positions.</summary>
    private static List<Disk> InitBoard(int size) {
        // Initialize game board with empty disks
        List<Disk> board = [.. Enumerable.Repeat(Disk.Empty, size * size)];
        // Set starting positions
        int row = size % 2 == 0 ? (size - 1) / 2 : (size - 1) / 2 - 1;
        int col = size / 2;
        board[row * size + row] = Disk.White;
        board[row * size + col] = Disk.Black;
        board[col * size + row] = Disk.Black;
        board[col * size + col] = Disk.White;
        return board;
    }

    /// <summary>Initialize empty squares for the board.</summary>
    private static List<Square> InitEmptySquares(int size, List<Disk> board) {
        return
        [
            .. Enumerable
                .Range(0, board.Count)
                .Where(i => board[i] == Disk.Empty)
                .Select(i => new Square(i % size, i / size)),
        ];
    }

    /// <summary>Format game board to string</summary>
    public override string ToString() {
        // Horizontal indices
        string text = $"  {ColorPrint.Bold(string.Join(" ", _indices))}";
        foreach (int y in _indices) {
            // Vertical index
            text += $"\n{ColorPrint.Bold(y)}";
            // Row values
            foreach (int x in _indices) {
                text += $" {_board[y * Size + x].BoardCharWithColor()}";
            }
        }
        return text;
    }
}
