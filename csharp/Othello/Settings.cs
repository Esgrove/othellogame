//==========================================================
// Settings
// Defines game settings
// Akseli Lukkarila
// 2019-2026
//==========================================================

namespace Othello {
    /// <summary>Game settings.</summary>
    public readonly struct Settings(
        int boardSize,
        bool autoplayMode,
        bool checkMode,
        bool showHelpers,
        bool showLog,
        bool testMode,
        bool useDefaults
        ) {
        public const int MinBoardSize = 4;
        public const int MaxBoardSize = 10;
        public const int DefaultBoardSize = 8;

        public int BoardSize { get; } = boardSize;
        public bool AutoplayMode { get; } = autoplayMode;
        public bool CheckMode { get; } = checkMode;
        public bool ShowHelpers { get; } = showHelpers;
        public bool ShowLog { get; } = showLog;
        public bool TestMode { get; } = testMode;
        public bool UseDefaults { get; } = useDefaults;

        /// <summary>Get default settings.</summary>
        public static Settings Default() {
            return new Settings(
                DefaultBoardSize,
                autoplayMode: false,
                checkMode: false,
                showHelpers: true,
                showLog: false,
                testMode: false,
                useDefaults: false
            );
        }

        /// <summary>Get player setting values from overall game settings.</summary>
        public PlayerSettings ToPlayerSettings() {
            return new PlayerSettings(ShowHelpers, CheckMode, TestMode);
        }

        public override string ToString() {
            return "Settings:"
                + $"\n  board_size: {BoardSize}"
                + $"\n  autoplay: {(AutoplayMode ? "true" : "false")}"
                + $"\n  check_mode: {(CheckMode ? "true" : "false")}"
                + $"\n  use_defaults: {(UseDefaults ? "true" : "false")}"
                + $"\n  show_helpers: {(ShowHelpers ? "true" : "false")}"
                + $"\n  show_log: {(ShowLog ? "true" : "false")}"
                + $"\n  test_mode: {(TestMode ? "true" : "false")}";
        }
    }

    /// <summary>Player settings.</summary>
    public readonly struct PlayerSettings(bool showHelpers, bool checkMode, bool testMode) {
        public bool ShowHelpers { get; } = showHelpers;
        public bool CheckMode { get; } = checkMode;
        public bool TestMode { get; } = testMode;

        /// <summary>Get default player settings.</summary>
        public static PlayerSettings Default() {
            return new PlayerSettings(showHelpers: true, checkMode: false, testMode: false);
        }
    }
}
