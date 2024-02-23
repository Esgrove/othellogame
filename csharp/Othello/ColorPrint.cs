//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2024
//==========================================================

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

        public static void WriteLine<T>(T text, Color color)
        {
            Console.WriteLine($"{text}".Pastel(color));
        }

        /// Print error message with red colour.
        public static void Error(string message)
        {
            var (indent, text) = SplitLeadingWhitespace(message);
            Console.WriteLine($"{indent}Error: {text}".Pastel(Color.Red));
        }

        /// Print warning message with yellow colour.
        public static void Warn(string message)
        {
            var (indent, text) = SplitLeadingWhitespace(message);
            Console.WriteLine($"{indent}Warning: {text}".Pastel(Color.Yellow));
        }

        /// Split a string into the leading whitespace and the rest of the string.
        private static (string, string) SplitLeadingWhitespace(string message)
        {
            // Find the index of the first non-whitespace character.
            int indentSize = 0;
            foreach (char c in message)
            {
                if (char.IsWhiteSpace(c))
                    indentSize++;
                else
                    break;
            }

            return (message[..indentSize], message[indentSize..]);
        }
    }
}
