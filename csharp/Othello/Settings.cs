//==========================================================
// Settings
// Defines game settings
// Akseli Lukkarila
// 2019-2025
//==========================================================

namespace Othello
{
    /// Game settings.
    public readonly struct Settings(
        int boardSize,
        bool autoplayMode,
        bool checkMode,
        bool showHelpers,
        bool showLog,
        bool testMode,
        bool useDefaultOptions
        )
    {
        public int BoardSize { get; } = boardSize;
        public bool AutoplayMode { get; } = autoplayMode;
        public bool CheckMode { get; } = checkMode;
        public bool ShowHelpers { get; } = showHelpers;
        public bool ShowLog { get; } = showLog;
        public bool TestMode { get; } = testMode;
        public bool UseDefaults { get; } = useDefaultOptions;

        /// Get player setting values from overall game settings.
        public PlayerSettings ToPlayerSettings()
        {
            return new PlayerSettings(CheckMode, ShowHelpers, TestMode);
        }
    }

    /// Player settings.
    public readonly struct PlayerSettings(bool checkMode, bool showHelpers, bool testMode)
    {
        public bool CheckMode { get; } = checkMode;
        public bool ShowHelpers { get; } = showHelpers;
        public bool TestMode { get; } = testMode;
    }
}
