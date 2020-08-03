using System;
using System.Collections.Generic;

namespace Othello
{
    public enum Disk : int
    {
        Black = -1,
        Empty = 0,
        White = 1
    }

    /// Represents one square location on the board.
    public struct Square
    {
        public Square(int x, int y) {
            X = x;
            Y = y;
        }

        public override int GetHashCode() {
            return HashCode.Combine(X, Y);
        }

        public override bool Equals(Object obj) {
            if (!(obj is Square square)) {
                return false;
            }
            return Equals(square);
        }

        public bool Equals(Square other) {
            return X.Equals(other.X) && Y.Equals(other.Y);
        }

        public override string ToString() {
            return $"({X},{Y})";
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
            return left.X < right.X || (left.X <= right.X && left.Y < right.Y);
        }

        public static bool operator >(Square left, Square right) {
            return left.X > right.X || (left.X >= right.X && left.Y > right.Y);
        }

        public readonly int X;
        public readonly int Y;
    }

    /// Represents one possible disk placement for given disk color.
    public readonly struct Move
    {
        public Move(Square square, int value, Disk disk, List<Square> directions) {
            Square = square;
            Value = value;
            Disk = disk;
            Directions = directions;
        }

        public override string ToString() {
            return $"Square: {Square} -> value: {Value}";
        }

        public static bool operator <(Move left, Move right) {
            return left.Value > right.Value || (left.Value == right.Value && left.Square < right.Square);
        }

        public static bool operator >(Move left, Move right) {
            return left.Value < right.Value || (left.Value == right.Value && left.Square > right.Square);
        }

        public readonly Square Square;
        public readonly int Value;
        public readonly Disk Disk;
        public readonly List<Square> Directions;
    }

    public static class Extensions
    {
        public static ConsoleColor Color(this Disk color) {
            if (color == Disk.Empty) {
                return ConsoleColor.White;
            }
            return color == Disk.White ? ConsoleColor.Cyan : ConsoleColor.DarkMagenta;
        }

        public static Disk OtherDisk(this Disk color) {
            if (color == Disk.Empty) {
                return Disk.Empty;
            }
            return color == Disk.White ? Disk.Black : Disk.White;
        }

        public static string Name(this Disk color) {
            return color.ToString().ToLower();
        }

        public static string BoardChar(this Disk color) {
            switch (color) {
                case Disk.White:
                    return "W";
                case Disk.Black:
                    return "B";
                default:
                    return "_";
            }
        }
    }
}