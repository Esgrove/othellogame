//==========================================================
// Class Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2026
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.Threading;

namespace Othello {
    /// <summary>Defines one player that can be either human or computer controlled.</summary>
    internal sealed class Player(Disk disk, PlayerSettings settings) {
        public bool CanPlay = true;
        internal Disk Disk { get; } = disk;
        private PlayerType _playerType = PlayerType.Human;
        internal int RoundsPlayed;
        internal readonly PlayerSettings Settings = settings;
        private readonly Random _random = new();

        /// <summary>Shorthand to initialize a new player for black disks.</summary>
        public static Player Black(PlayerSettings settings) {
            return new Player(Disk.Black, settings);
        }

        /// <summary>Shorthand to initialize a new player for white disks.</summary>
        public static Player White(PlayerSettings settings) {
            return new Player(Disk.White, settings);
        }

#nullable enable
        /// <summary>Play one round as this player.</summary>
        public string? PlayOneMove(Board board) {
            if (!Settings.CheckMode) {
                Console.WriteLine($"Turn: {Disk.DiskString()}");
            }
            List<Move> moves = board.PossibleMoves(Disk);
            if (moves.Count == 0) {
                CanPlay = false;
                if (!Settings.CheckMode) {
                    ColorPrint.WriteLine("  No moves available...", Color.Yellow);
                }
                return null;
            }

            CanPlay = true;
            if (Human() && Settings is { ShowHelpers: true, CheckMode: false }) {
                board.PrintPossibleMoves(moves);
            }
            Move chosenMove = Human() ? GetHumanMove(moves) : GetComputerMove(moves);
            board.PlaceDisk(chosenMove);
            if (!Settings.CheckMode) {
                board.PrintScore();
            }
            ++RoundsPlayed;
            if (!Settings.TestMode) {
                Thread.Sleep(1000);
            }
            return chosenMove.LogEntry();
        }
#nullable disable

        /// <summary>Reset player status for a new game.</summary>
        public void Reset() {
            CanPlay = true;
            RoundsPlayed = 0;
        }

        /// <summary>Returns true if player is controlled by a human player.</summary>
        public bool Human() {
            return _playerType.Human();
        }

        /// <summary>Returns true if player is controlled by computer.</summary>
        public bool Computer() {
            return _playerType.Computer();
        }

        /// <summary>Set the player as human or computer controlled.</summary>
        public void SetPlayerType(PlayerType playerType) {
            _playerType = playerType;
        }

        /// <summary>Set the player as human controlled.</summary>
        public void SetHuman() {
            SetPlayerType(PlayerType.Human);
        }

        /// <summary>Set the player as computer controlled.</summary>
        public void SetComputer() {
            SetPlayerType(PlayerType.Computer);
        }

        /// <summary>Return move chosen by computer.</summary>
        private Move GetComputerMove(List<Move> moves) {
            if (!Settings.CheckMode) {
                Console.WriteLine("  Computer plays...");
            }
            Move chosenMove;
            if (Settings.TestMode) {
                chosenMove = moves[0];
            } else {
                // Wait a bit and pick a random move
                Thread.Sleep(_random.Next(1000, 2000));
                chosenMove = moves[_random.Next(moves.Count)];
            }
            if (!Settings.CheckMode) {
                Console.WriteLine($"  {chosenMove.Square} -> {chosenMove.Value}");
            }
            return chosenMove;
        }

        /// <summary>Return move chosen by a human player.</summary>
        private Move GetHumanMove(List<Move> moves) {
            while (true) {
                Square square = GetSquare();
                // Check that the chosen square is actually one of the possible moves
                if (moves.Exists(move => square.Equals(move.Square))) {
                    return moves.Find(move => square.Equals(move.Square));
                }
                ColorPrint.PrintError($"  Can't place a {Disk.DiskString()} disk in square {square}!");
            }
        }

        /// <summary>Ask human player for square coordinates.</summary>
        private static Square GetSquare() {
            while (true) {
                Console.Write("  Give disk position (x,y): ");
                string input = Console.ReadLine();
                if (input != null) {
                    string[] values = input.Trim().Split(',');
                    if (values.Length == 2
                        && int.TryParse(values[0], NumberStyles.None, CultureInfo.InvariantCulture, out int x)
                        && int.TryParse(values[1], NumberStyles.None, CultureInfo.InvariantCulture, out int y)
                        && x >= 0
                        && y >= 0
                    ) {
                        return new Square(x, y);
                    }
                }
                ColorPrint.PrintError("  Give coordinates in the form 'x,y'!");
            }
        }

        /// <summary>Return player type description string.</summary>
        public string TypeString() {
            return _playerType.TypeString();
        }

        public override string ToString() {
            return $"{Disk.DiskString()} | {TypeString()} | Moves: {RoundsPlayed}";
        }
    }

    /// <summary>Player can be controlled either by a human or computer.</summary>
    public enum PlayerType {
        Human,
        Computer,
    }

    public static class PlayerTypeExtensions {
        extension(PlayerType playerType) {
            /// <summary>Check if the player is controlled by a human.</summary>
            public bool Human() {
                return playerType == PlayerType.Human;
            }

            /// <summary>Check if the player is controlled by a computer.</summary>
            public bool Computer() {
                return playerType == PlayerType.Computer;
            }

            /// <summary>Return player type description string.</summary>
            public string TypeString() {
                return playerType == PlayerType.Human ? "Human   " : "Computer";
            }
        }
    }
}
