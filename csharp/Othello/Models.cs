//==========================================================
// Models
// Basic types and methods.
// Akseli Lukkarila
// 2019-2025
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;

namespace Othello {
    /// Represents one game piece or lack of one.
    public enum Disk {
        Black = -1,
        Empty = 0,
        White = 1,
    }

    /// Represents one step direction on the board.
    [System.Diagnostics.CodeAnalysis.SuppressMessage("Naming", "CA1716:Identifiers should not match keywords", Justification = "Not relevant in this project")]
    public readonly struct Step(int x, int y) : IEquatable<Step> {
        public int X { get; } = x;
        public int Y { get; } = y;

        public override int GetHashCode() {
            return HashCode.Combine(X, Y);
        }

        public override bool Equals(object obj) {
            return obj is Step other && Equals(other);
        }

        public bool Equals(Step other) {
            return X == other.X && Y == other.Y;
        }

        public override string ToString() {
            return $"[{X},{Y}]";
        }

        public void Deconstruct(out int x, out int y) {
            x = X;
            y = Y;
        }

        public static bool operator ==(Step left, Step right) {
            return left.Equals(right);
        }

        public static bool operator !=(Step left, Step right) {
            return !(left == right);
        }
    }

    /// Represents one square location on the board.
    public readonly struct Square(int x, int y) : IComparable<Square>, IEquatable<Square> {
        public int X { get; } = x;
        public int Y { get; } = y;

        public static Square operator +(Square square, Step step) {
            return new Square(square.X + step.X, square.Y + step.Y);
        }

        public override int GetHashCode() {
            return HashCode.Combine(X, Y);
        }

        public override bool Equals(object obj) {
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

    /// Represents a continuous line of squares in one direction.
    ///
    /// The step component determines the direction on the board,
    /// and count describes how many consecutive squares in that direction there are.
    public readonly struct Direction(Step step, int count) : IEquatable<Direction>, IComparable<Direction> {
        public Step Step { get; } = step;
        public int Count { get; } = count;

        public void Deconstruct(out Step step, out int count) {
            step = Step;
            count = Count;
        }

        public override int GetHashCode() {
            return HashCode.Combine(Step, Count);
        }

        public override bool Equals(object obj) {
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

    /// Represents one possible disk placement for the given disk colour.
    public readonly struct Move(Square square, int value, Disk disk, List<Direction> directions) : IComparable<Move>, IEquatable<Move> {
        public Square Square { get; } = square;
        public int Value { get; } = value;
        public Disk Disk { get; } = disk;
        public List<Direction> Directions { get; } = directions;

        /// Format move for log entry
        public string LogEntry() {
            return $"{Disk.BoardChar(false)}:{Square},{Value}";
        }

        /// Get all the squares playing this move will change.
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

        public override bool Equals(object obj) {
            return obj is Move other && Equals(other);
        }

        public override int GetHashCode() {
            return HashCode.Combine(Square, Value, (int)Disk, Directions);
        }

        public static bool operator ==(Move left, Move right) {
            return left.Equals(right);
        }

        public static bool operator !=(Move left, Move right) {
            return !(left == right);
        }

        public static bool operator <=(Move left, Move right) {
            return left.CompareTo(right) <= 0;
        }

        public static bool operator >=(Move left, Move right) {
            return left.CompareTo(right) >= 0;
        }

        public bool Equals(Move other) {
            return Square.Equals(other.Square) && Value == other.Value && Disk == other.Disk && Equals(Directions, other.Directions);
        }
    }

    public static class Extensions {
        extension(Disk disk) {
            public Color DiskColor() {
                if (disk == Disk.Empty) {
                    return Color.White;
                }
                return disk == Disk.White ? Color.Cyan : Color.Magenta;
            }

            public Disk Opponent() {
                if (disk == Disk.Empty) {
                    return Disk.Empty;
                }
                return disk == Disk.White ? Disk.Black : Disk.White;
            }

            public string Name() {
                return ColorPrint.Get(disk.ToString().ToUpperInvariant(), disk.DiskColor());
            }

            public string BoardChar(bool color = true) {
                if (disk == Disk.Empty) {
                    return "_";
                }

                string diskChar = disk == Disk.White ? "W" : "B";
                return color ? ColorPrint.Get(diskChar, disk.DiskColor()) : diskChar;
            }
        }
    }
}
