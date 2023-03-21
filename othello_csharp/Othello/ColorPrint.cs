using System;
using System.Drawing;
using Pastel;

namespace Othello
{
    public static class ColorPrint
    {
        public static string Get<T>(T text, Color color)
        {
            return $"{text}".Pastel(color);
        }

        public static void Write<T>(T text, Color color)
        {
            Console.Write($"{text}".Pastel(color));
        }
        public static void Error(string message, int indent = 0)
        {
            var whitespace = indent > 0 ? new string(' ', indent) : "";
            Console.WriteLine($"{whitespace}{"Error:".Pastel(Color.Red)} {message}");
        }
    }
}
