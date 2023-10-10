//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2023
//==========================================================

using Pastel;
using System;
using System.Drawing;

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
            Console.WriteLine($"{indent}Error: {message}".Pastel(Color.Red));
        }

        /// Print warning message with yellow colour.
        public static void Warn(string message)
        {
            var (indent, text) = SplitLeadingWhitespace(message);
            Console.WriteLine($"{indent}Warning: {text}".Pastel(Color.Yellow));
        }

        /// Split a string to the leading whitespace and rest of the string.
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
