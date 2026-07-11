//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2026
//==========================================================

using Pastel;
using System;
using System.Drawing;

namespace Othello;

/// <summary>Interface for coloured terminal printing.</summary>
public static class ColorPrint {
    private static readonly bool UseColor;

    static ColorPrint() {
        // Disable colours when output is redirected or NO_COLOR is set,
        // matching the behaviour of the Rust `colored` crate.
        UseColor =
            !Console.IsOutputRedirected
            && string.IsNullOrEmpty(Environment.GetEnvironmentVariable("NO_COLOR"));
        if (!UseColor) {
            ConsoleExtensions.Disable();
        }
    }

    /// <summary>Format text with the given colour.</summary>
    public static string Get<T>(T text, Color color) {
        return $"{text}".Pastel(color);
    }

    /// <summary>Format text with bold style.</summary>
    public static string Bold<T>(T text) {
        return UseColor ? $"\x1b[1m{text}\x1b[0m" : $"{text}";
    }

    /// <summary>Write text with the given colour without a trailing newline.</summary>
    public static void Write<T>(T text, Color color) {
        Console.Write($"{text}".Pastel(color));
    }

    /// <summary>Print text with the given colour.</summary>
    public static void WriteLine<T>(T text, Color color) {
        Console.WriteLine($"{text}".Pastel(color));
    }

    /// <summary>Print text with bold style.</summary>
    public static void PrintBold<T>(T text) {
        Console.WriteLine(Bold(text));
    }

    /// <summary>Print text with green colour.</summary>
    public static void PrintGreen<T>(T text) {
        Console.WriteLine($"{text}".Pastel(Color.Green));
    }

    /// <summary>Print text with green colour and bold style.</summary>
    public static void PrintGreenBold<T>(T text) {
        Console.WriteLine(Bold($"{text}".Pastel(Color.Green)));
    }

    /// <summary>Print text with yellow colour.</summary>
    public static void PrintYellow<T>(T text) {
        Console.WriteLine($"{text}".Pastel(Color.Yellow));
    }

    /// <summary>Print text with yellow colour and bold style.</summary>
    public static void PrintYellowBold<T>(T text) {
        Console.WriteLine(Bold($"{text}".Pastel(Color.Yellow)));
    }

    /// <summary>Print text with red colour.</summary>
    public static void PrintRed<T>(T text) {
        Console.WriteLine($"{text}".Pastel(Color.Red));
    }

    /// <summary>Print text with red colour and bold style.</summary>
    public static void PrintRedBold<T>(T text) {
        Console.WriteLine(Bold($"{text}".Pastel(Color.Red)));
    }

    /// <summary>Print error message with red colour.</summary>
    public static void PrintError(string message) {
        (string indent, string text) = SplitLeadingWhitespace(message);
        Console.WriteLine($"{indent}Error: {text}".Pastel(Color.Red));
    }

    /// <summary>Print warning message with yellow colour.</summary>
    public static void PrintWarn(string message) {
        (string indent, string text) = SplitLeadingWhitespace(message);
        Console.WriteLine($"{indent}Warning: {text}".Pastel(Color.Yellow));
    }

    /// <summary>Split a string into the leading whitespace and the rest of the string.</summary>
    private static (string, string) SplitLeadingWhitespace(string message) {
        // Find the index of the first non-whitespace character.
        int indentSize = 0;
        foreach (char c in message) {
            if (char.IsWhiteSpace(c)) {
                indentSize++;
            } else {
                break;
            }
        }

        return (message[..indentSize], message[indentSize..]);
    }
}
