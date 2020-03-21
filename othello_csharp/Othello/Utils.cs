using System;

namespace Othello
{
    public struct Square
    {
        public Square(int x, int y)
        {
            X = x;
            Y = y;
        }

        public override string ToString()
        {
            return $"({X},{Y})";
        }

        public readonly int X;
        public readonly int Y;
    }
    public struct Move
    {
        public Move(int x, int y, int value)
        {
            Square = new Square(x, y);
            Value = value;
        }
        public override string ToString()
        {
            return $"point: {Square} -> value: {Value}";
        }

        public readonly Square Square;
        public readonly int Value;
    }
    public enum Disk // : int
    {
        Black = -1,
        Empty = 0,
        White = 1,
        Invalid = 2
    }

    public static class Extensions
    {
        public static ConsoleColor DiskColor(this Disk color)
        {
            if (color == Disk.Empty)
            {
                return ConsoleColor.White;
            }

            return color == Disk.White ? ConsoleColor.DarkMagenta : ConsoleColor.Cyan;
        }

        public static Disk OtherDisk(this Disk color) {
            if (color == Disk.Empty)
            {
                return Disk.Empty;
            }
            return color == Disk.White ? Disk.Black : Disk.White;
        }

        public static string Name(this Disk color)
        {
            return color.ToString().ToLower();
        }

        public static string GetBoardChar(this Disk color)
        {
            switch (color)
            {
                case Disk.White:
                    return "W";
                case Disk.Black:
                    return "B";
                case Disk.Empty:
                    return "_";
                default:
                    throw new ArgumentException("Disk is invalid!");
            }
        }
    }
}