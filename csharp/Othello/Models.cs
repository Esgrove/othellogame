//==========================================================
// Models
// Basic types and methods.
// Akseli Lukkarila
// 2019-2025
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;

namespace Othello
{
    /// Represents one game piece or lack of one.
    public enum Disk
    {
        Black = -1,
        Empty = 0,
        White = 1,
    }

    /// Represents one step direction on the board.
    public readonly struct Step : IEquatable<Step>
    {
        public readonly int X;
        public readonly int Y;

        public Step(int x, int y)
        {
            X = x;
            Y = y;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(X, Y);
        }

        public override bool Equals(object obj)
        {
            return obj is Step other && Equals(other);
        }

        public bool Equals(Step other)
        {
            return X == other.X && Y == other.Y;
        }

        public override string ToString()
        {
            return $"[{X},{Y}]";
        }

        public void Deconstruct(out int x, out int y)
        {
            x = X;
            y = Y;
        }

        public static bool operator ==(Step left, Step right)
        {
            return left.Equals(right);
        }

        public static bool operator !=(Step left, Step right)
        {
            return !(left == right);
        }
    }

    /// Represents one square location on the board.
    public readonly struct Square : IComparable<Square>, IEquatable<Square>
    {
        public readonly int X;
        public readonly int Y;

        public Square(int x, int y)
        {
            X = x;
            Y = y;
        }

        public static Square operator +(Square square, Step step)
        {
            return new Square(square.X + step.X, square.Y + step.Y);
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(X, Y);
        }

        public override bool Equals(Object obj)
        {
            return obj is Square other && Equals(other);
        }

        public bool Equals(Square other)
        {
            return X == other.X && Y == other.Y;
        }

        public override string ToString()
        {
            return $"({X},{Y})";
        }

        public void Deconstruct(out int x, out int y)
        {
            x = X;
            y = Y;
        }

        public int CompareTo(Square other)
        {
            return X == other.X ? Y.CompareTo(other.Y) : X.CompareTo(other.X);
        }

        public static Square operator +(Square left, Square right)
        {
            return new Square(left.X + right.X, left.Y + right.Y);
        }

        public static bool operator ==(Square left, Square right)
        {
            return left.Equals(right);
        }

        public static bool operator !=(Square left, Square right)
        {
            return !left.Equals(right);
        }

        public static bool operator <(Square left, Square right)
        {
            return left.X < right.X || (left.X <= right.X && left.Y < right.Y);
        }

        public static bool operator >(Square left, Square right)
        {
            return left.X > right.X || (left.X >= right.X && left.Y > right.Y);
        }
    }

    /// Represents one possible disk placement for the given disk color.
    public readonly struct Move : IComparable<Move>
    {
        public readonly Square Square;
        public readonly int Value;
        public readonly Disk Disk;
        public readonly List<(Step step, int count)> Directions;

        public Move(Square square, int value, Disk disk, List<(Step, int)> directions)
        {
            Square = square;
            Value = value;
            Disk = disk;
            Directions = directions;
        }

        /// Format move for log entry
        public string LogEntry()
        {
            return $"{Disk.BoardChar(false)}:{Square},{Value}";
        }

        /// Get all the squares playing this move will change.
        public List<Square> AffectedSquares()
        {
            List<Square> paths = new();
            foreach (var (step, num) in Directions)
            {
                Square pos = Square + step;
                for (int i = 0; i < num; i++)
                {
                    paths.Add(pos);
                    pos += step;
                }
            }
            paths.Sort();
            return paths;
        }

        public override string ToString()
        {
            return $"Square: {Square} -> value: {Value}";
        }

        public int CompareTo(Move other)
        {
            int value = other.Value.CompareTo(Value);
            return value == 0 ? Square.CompareTo(other.Square) : value;
        }

        public static bool operator <(Move left, Move right)
        {
            return left.Value > right.Value
                || (left.Value == right.Value && left.Square < right.Square);
        }

        public static bool operator >(Move left, Move right)
        {
            return left.Value < right.Value
                || (left.Value == right.Value && left.Square > right.Square);
        }
    }

    public static class Extensions
    {
        public static Color DiskColor(this Disk disk)
        {
            if (disk == Disk.Empty)
            {
                return Color.White;
            }
            return disk == Disk.White ? Color.Cyan : Color.Magenta;
        }

        public static Disk Opponent(this Disk disk)
        {
            if (disk == Disk.Empty)
            {
                return Disk.Empty;
            }
            return disk == Disk.White ? Disk.Black : Disk.White;
        }

        public static string Name(this Disk disk)
        {
            return ColorPrint.Get(disk.ToString().ToUpper(), disk.DiskColor());
        }

        public static string BoardChar(this Disk disk, bool color = true)
        {
            if (disk == Disk.Empty)
            {
                return "_";
            }

            string diskChar = disk == Disk.White ? "W" : "B";
            if (color)
            {
                return ColorPrint.Get(diskChar, disk.DiskColor());
            }

            return diskChar;
        }
    }
}
