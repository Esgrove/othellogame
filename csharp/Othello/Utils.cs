//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2024
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Security.Cryptography;
using System.Text;

namespace Othello
{
    /// Represents one game piece or lack of one.
    public enum Disk : int
    {
        Black = -1,
        Empty = 0,
        White = 1
    }

    /// Represents one step direction on the board.
    public readonly struct Step
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

        public override bool Equals(Object obj)
        {
            if (obj is not Step step)
            {
                return false;
            }
            return Equals(step);
        }

        public bool Equals(Step other)
        {
            var (x, y) = other;
            return X.Equals(x) && Y.Equals(y);
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
    public readonly struct Square : IComparable<Square>
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
            if (obj is not Square square)
            {
                return false;
            }
            return Equals(square);
        }

        public bool Equals(Square other)
        {
            var (x, y) = other;
            return X.Equals(x) && Y.Equals(y);
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
            if (X == other.X)
            {
                return Y.CompareTo(other.Y);
            }

            return X.CompareTo(other.X);
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
            foreach (var (step, num) in this.Directions)
            {
                Square pos = this.Square + step;
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
            var value = other.Value.CompareTo(Value);
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

    public readonly struct PlayerSettings
    {
        public bool ShowHelpers { get; }
        public bool TestMode { get; }

        public PlayerSettings(bool showHelpers, bool testMode)
        {
            ShowHelpers = showHelpers;
            TestMode = testMode;
        }
    }

    public readonly struct Settings
    {
        public int BoardSize { get; }
        public bool AutoplayMode { get; }
        public bool UseDefaults { get; }
        public bool ShowHelpers { get; }
        public bool ShowLog { get; }
        public bool TestMode { get; }

        public Settings(
            int boardSize,
            bool autoplayMode,
            bool useDefaultOptions,
            bool showHelpers,
            bool showLog,
            bool testMode
        )
        {
            BoardSize = boardSize;
            AutoplayMode = autoplayMode;
            UseDefaults = useDefaultOptions;
            ShowHelpers = showHelpers;
            ShowLog = showLog;
            TestMode = testMode;
        }

        public PlayerSettings ToPlayerSettings()
        {
            return new PlayerSettings(ShowHelpers, TestMode);
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
            else
            {
                return diskChar;
            }
        }
    }

    // Can't have a function without a class :(
    public static class Utils
    {
        public static string CalculateSHA256(string input)
        {
            byte[] inputBytes = Encoding.UTF8.GetBytes(input);
            byte[] hashBytes = SHA256.HashData(inputBytes);

            StringBuilder builder = new();
            foreach (byte b in hashBytes)
            {
                builder.Append(b.ToString("x2"));
            }

            return builder.ToString();
        }

        public static string VersionInfo()
        {
            return $"{Version.VersionNumber} {Version.BuildTime} {Version.GitCommit} {Version.GitBranch}";
        }
    }
}
