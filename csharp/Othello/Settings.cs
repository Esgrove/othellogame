//==========================================================
// Settings
// Defines game settings
// Akseli Lukkarila
// 2019-2025
//==========================================================

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Security.Cryptography;
using System.Text;

namespace Othello
{
    /// Game settings.
    public readonly struct Settings
    {
        public int BoardSize { get; }
        public bool AutoplayMode { get; }
        public bool CheckMode { get; }
        public bool ShowHelpers { get; }
        public bool ShowLog { get; }
        public bool TestMode { get; }
        public bool UseDefaults { get; }

        public Settings(
            int boardSize,
            bool autoplayMode,
            bool checkMode,
            bool showHelpers,
            bool showLog,
            bool testMode,
            bool useDefaultOptions
        )
        {
            BoardSize = boardSize;
            AutoplayMode = autoplayMode;
            CheckMode = checkMode;
            ShowHelpers = showHelpers;
            ShowLog = showLog;
            TestMode = testMode;
            UseDefaults = useDefaultOptions;
        }

        /// Get player setting values from overall game settings.
        public PlayerSettings ToPlayerSettings()
        {
            return new PlayerSettings(CheckMode, ShowHelpers, TestMode);
        }
    }

    /// Player settings.
    public readonly struct PlayerSettings
    {
        public bool CheckMode { get; }
        public bool ShowHelpers { get; }
        public bool TestMode { get; }

        public PlayerSettings(bool checkMode, bool showHelpers, bool testMode)
        {
            CheckMode = checkMode;
            ShowHelpers = showHelpers;
            TestMode = testMode;
        }
    }
}
