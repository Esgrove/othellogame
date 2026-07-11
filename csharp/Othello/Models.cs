//==========================================================
// Models
// Basic types and methods.
// Akseli Lukkarila
// 2019-2026
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;

namespace Othello;

/// <summary>Represents one game piece or lack of one.</summary>
public enum Disk {
    Black = -1,
    Empty = 0,
    White = 1,
}

/// <summary>Represents one step direction on the board.</summary>
[System.Diagnostics.CodeAnalysis.SuppressMessage("Naming", "CA1716:Identifiers should not match keywords", Justification = "Not relevant in this project")]
public readonly record struct Step(int X, int Y) {
    public override string ToString() {
        return $"[{X},{Y}]";
    }

    public static Step operator +(Step left, Step right) {
        return new Step(left.X + right.X, left.Y + right.Y);
    }
}

/// <summary>Represents one square location on the board.</summary>
public readonly struct Square(int x, int y) : IComparable<Square>, IEquatable<Square> {
    public int X { get; } = x;
    public int Y { get; } = y;

    /// <summary>Get the index of this square on the board.</summary>
    public int BoardIndex(int boardSize) {
        return Y * boardSize + X;
    }

    public static Square operator +(Square square, Step step) {
        return new Square(square.X + step.X, square.Y + step.Y);
    }

    public override int GetHashCode() {
        return HashCode.Combine(X, Y);
    }

    public override bool Equals(object? obj) {
        return obj is Square other && Equals(other);
    }

    public bool Equals(Square other) {
        return X == other.X && Y == other.Y;
    }

    public override string ToString() {
        return $"({X},{Y})";
    }

    public void Deconstruct(out int x, out int y) {
        x = X;
        y = Y;
    }

    public int CompareTo(Square other) {
        return X == other.X ? Y.CompareTo(other.Y) : X.CompareTo(other.X);
    }

    public static Square operator +(Square left, Square right) {
        return new Square(left.X + right.X, left.Y + right.Y);
    }

    public static bool operator ==(Square left, Square right) {
        return left.Equals(right);
    }

    public static bool operator !=(Square left, Square right) {
        return !left.Equals(right);
    }

    public static bool operator <(Square left, Square right) {
        return left.CompareTo(right) < 0;
    }

    public static bool operator >(Square left, Square right) {
        return left.CompareTo(right) > 0;
    }

    public static bool operator <=(Square left, Square right) {
        return left.CompareTo(right) <= 0;
    }

    public static bool operator >=(Square left, Square right) {
        return left.CompareTo(right) >= 0;
    }
}

/// <summary>
/// Represents a continuous line of squares in one direction.
///
/// The step field determines the direction on the board,
/// and count describes how many consecutive squares in that direction there are.
/// </summary>
public readonly struct Direction(Step step, int count) : IEquatable<Direction>, IComparable<Direction> {
    /// <summary>Direction of travel on the board</summary>
    public Step Step { get; } = step;

    /// <summary>Number of consecutive same colour squares along this direction</summary>
    public int Count { get; } = count;

    public void Deconstruct(out Step step, out int count) {
        step = Step;
        count = Count;
    }

    public override int GetHashCode() {
        return HashCode.Combine(Step, Count);
    }

    public override bool Equals(object? obj) {
        return obj is Direction other && Equals(other);
    }

    public bool Equals(Direction other) {
        return Step == other.Step && Count == other.Count;
    }

    public override string ToString() {
        return $"{Step}:{Count}";
    }

    public int CompareTo(Direction other) {
        int stepX = Step.X.CompareTo(other.Step.X);
        if (stepX != 0) {
            return stepX;
        }

        int stepY = Step.Y.CompareTo(other.Step.Y);
        return stepY != 0 ? stepY : Count.CompareTo(other.Count);
    }

    public static bool operator ==(Direction left, Direction right) {
        return left.Equals(right);
    }

    public static bool operator !=(Direction left, Direction right) {
        return !(left == right);
    }

    public static bool operator <(Direction left, Direction right) {
        return left.CompareTo(right) < 0;
    }

    public static bool operator >(Direction left, Direction right) {
        return left.CompareTo(right) > 0;
    }

    public static bool operator <=(Direction left, Direction right) {
        return left.CompareTo(right) <= 0;
    }

    public static bool operator >=(Direction left, Direction right) {
        return left.CompareTo(right) >= 0;
    }
}

/// <summary>Represents one possible disk placement for the given disk colour.</summary>
public readonly record struct Move(Square Square, Disk Disk, int Value, List<Direction> Directions) : IComparable<Move> {
    /// <summary>Format move for log entry</summary>
    public string LogEntry() {
        return $"{Disk.BoardChar()}:{Square},{Value}";
    }

    /// <summary>Get all the squares playing this move will change.</summary>
    public List<Square> AffectedSquares() {
        List<Square> paths = [];
        foreach ((Step step, int count) in Directions) {
            Square pos = Square + step;
            for (int i = 0; i < count; i++) {
                paths.Add(pos);
                pos += step;
            }
        }
        paths.Sort();
        return paths;
    }

    public override string ToString() {
        return $"Square: {Square} -> value: {Value}";
    }

    public int CompareTo(Move other) {
        int value = other.Value.CompareTo(Value);
        return value == 0 ? Square.CompareTo(other.Square) : value;
    }

    public static bool operator <(Move left, Move right) {
        return left.Value > right.Value
            || (left.Value == right.Value && left.Square < right.Square);
    }

    public static bool operator >(Move left, Move right) {
        return left.Value < right.Value
            || (left.Value == right.Value && left.Square > right.Square);
    }

    public override int GetHashCode() {
        return HashCode.Combine(Square, Value, (int)Disk);
    }

    public static bool operator <=(Move left, Move right) {
        return left.CompareTo(right) <= 0;
    }

    public static bool operator >=(Move left, Move right) {
        return left.CompareTo(right) >= 0;
    }

    // Custom equality that excludes directions to match the Rust implementation.
    public bool Equals(Move other) {
        return Square.Equals(other.Square) && Value == other.Value && Disk == other.Disk;
    }
}

/// <summary>Extension methods for the disk enum.</summary>
public static class Extensions {
    extension(Disk disk) {
        /// <summary>Returns a single character identifier string for the given disk.</summary>
        public string BoardChar() {
            return disk switch {
                Disk.Black => "B",
                Disk.Empty => "_",
                Disk.White => "W",
                _ => throw new ArgumentOutOfRangeException(nameof(disk)),
            };
        }

        /// <summary>Returns a coloured single character identifier string for the given disk.</summary>
        public string BoardCharWithColor() {
            return ColorPrint.Get(disk.BoardChar(), disk.DiskColor());
        }

        /// <summary>Return the associated colour for this disk.</summary>
        public Color DiskColor() {
            return disk switch {
                Disk.Black => Color.Magenta,
                Disk.Empty => Color.White,
                Disk.White => Color.Cyan,
                _ => throw new ArgumentOutOfRangeException(nameof(disk)),
            };
        }

        /// <summary>Returns the disk formatted as a coloured string.</summary>
        public string DiskString() {
            return disk switch {
                Disk.Black => ColorPrint.Get("BLACK", disk.DiskColor()),
                Disk.Empty => ColorPrint.Get("EMPTY", disk.DiskColor()),
                Disk.White => ColorPrint.Get("WHITE", disk.DiskColor()),
                _ => throw new ArgumentOutOfRangeException(nameof(disk)),
            };
        }

        /// <summary>Return the opposing disk colour for this disk.</summary>
        public Disk Opponent() {
            return disk switch {
                Disk.Black => Disk.White,
                Disk.Empty => Disk.Empty,
                Disk.White => Disk.Black,
                _ => throw new ArgumentOutOfRangeException(nameof(disk)),
            };
        }
    }
}
