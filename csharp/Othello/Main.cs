//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

using System;
using System.CommandLine;
using System.Linq;

namespace Othello;

/// <summary>Command line program entry point.</summary>
public static class Program {
    public static int Main(string[] args) {
        Argument<int?> sizePositional = new(name: "size") {
            Description = $"Optional board size ({Settings.MinBoardSize}..{Settings.MaxBoardSize})"
        };

        Option<bool> autoplayOption = new(name: "--autoplay", aliases: ["-a"]) {
            Description = "Enable autoplay mode with computer control"
        };

        Option<bool> checkModeOption = new(name: "--check", aliases: ["-c"]) {
            Description = "Autoplay and only print result"
        };

        Option<bool> useDefaultSettingsOption = new(name: "--default", aliases: ["-d"]) {
            Description = "Play with default settings"
        };

        Option<bool> showLogOption = new(name: "--log", aliases: ["-l"]) {
            Description = "Show game log at the end"
        };

        Option<bool> hideHelpersOption = new(name: "--no-helpers", aliases: ["-n"]) {
            Description = "Hide disk placement hints"
        };

        Option<bool> testModeOption = new(name: "--test", aliases: ["-t"]) {
            Description = "Enable test mode with deterministic computer moves"
        };

        Option<bool> versionFlag = new(name: "--version", aliases: ["-v"]) {
            Description = "Print version and exit"
        };

        RootCommand rootCommand = new("A simple Othello CLI game implementation in C#");

        // Remove default version option
        Option? defaultVersionOption = rootCommand.Options.FirstOrDefault(opt =>
            opt.Name == "--version"
            || opt.Aliases.Contains("--version")
            || opt.Aliases.Contains("-v")
        );
        if (defaultVersionOption != null) {
            rootCommand.Options.Remove(defaultVersionOption);
        }

        rootCommand.Arguments.Add(sizePositional);
        rootCommand.Options.Add(autoplayOption);
        rootCommand.Options.Add(checkModeOption);
        rootCommand.Options.Add(useDefaultSettingsOption);
        rootCommand.Options.Add(showLogOption);
        rootCommand.Options.Add(hideHelpersOption);
        rootCommand.Options.Add(testModeOption);
        rootCommand.Options.Add(versionFlag);

        // `--autoplay` and `--default` are mutually exclusive
        rootCommand.Validators.Add(result => {
            if (result.GetValue(autoplayOption) && result.GetValue(useDefaultSettingsOption)) {
                result.AddError("The option '--autoplay' cannot be used with '--default'");
            }
        });

        rootCommand.SetAction(
            parseResult => {
                int? size = parseResult.GetValue(sizePositional);
                bool autoplay = parseResult.GetValue(autoplayOption);
                bool checkMode = parseResult.GetValue(checkModeOption);
                bool useDefaultSettings = parseResult.GetValue(useDefaultSettingsOption);
                bool showLog = parseResult.GetValue(showLogOption);
                bool hideHelpers = parseResult.GetValue(hideHelpersOption);
                bool testMode = parseResult.GetValue(testModeOption);
                bool version = parseResult.GetValue(versionFlag);

                if (version) {
                    Console.WriteLine(Version.VersionInfo());
                    Environment.Exit(0);
                }

                ColorPrint.PrintGreenBold("OTHELLO GAME - C#");

                int boardSize = ResolveBoardSize(size, autoplay, useDefaultSettings);

                Settings settings = new(
                    boardSize,
                    autoplay || checkMode,
                    checkMode,
                    !hideHelpers,
                    showLog || checkMode,
                    testMode || checkMode,
                    useDefaultSettings
                );

                new Othello(settings).Play();
            }
        );

        try {
            ParseResult parseResult = rootCommand.Parse(args);
            return parseResult.Invoke();
        } catch (OperationCanceledException) {
            Console.WriteLine("\ncancelled...");
            return 1;
        } catch (Exception ex) {
            ColorPrint.PrintError($"An exception occurred: {ex}");
            return 1;
        }
    }

    /// <summary>Resolve the board size to use from CLI arguments, or by asking the user.</summary>
    private static int ResolveBoardSize(int? size, bool autoplay, bool useDefaultSettings) {
        // Try to read board size from command line args
        if (size != null) {
            int boardSize = size.Value;
            if (boardSize is < Settings.MinBoardSize or > Settings.MaxBoardSize) {
                ColorPrint.PrintError($"Unsupported board size: {boardSize}");
                Environment.Exit(1);
            }
            Console.WriteLine($"Using board size: {boardSize}");
            return boardSize;
        }

        if (autoplay || useDefaultSettings) {
            return Settings.DefaultBoardSize;
        }

        // Otherwise ask user for board size
        return Othello.GetBoardSize();
    }
}
