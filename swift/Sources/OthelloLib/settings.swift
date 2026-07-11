//==========================================================
// Settings
// Defines game and player settings.
// Akseli Lukkarila
// 2019-2026
//==========================================================

/// Minimum allowed board size.
public let MIN_BOARD_SIZE: Int = 4
/// Maximum allowed board size.
public let MAX_BOARD_SIZE: Int = 10
/// Default board size when none is given.
public let DEFAULT_BOARD_SIZE: Int = 8

/// Game settings.
public struct Settings {
    var boardSize: Int
    var autoplayMode: Bool
    var checkMode: Bool
    var showHelpers: Bool
    var showLog: Bool
    var testMode: Bool
    var useDefaults: Bool

    /// Initialize game settings with the given values.
    public init(
        boardSize: Int,
        autoplayMode: Bool,
        checkMode: Bool,
        showHelpers: Bool,
        showLog: Bool,
        testMode: Bool,
        useDefaults: Bool
    ) {
        self.boardSize = boardSize
        self.autoplayMode = autoplayMode
        self.checkMode = checkMode
        self.showHelpers = showHelpers
        self.showLog = showLog
        self.testMode = testMode
        self.useDefaults = useDefaults
    }

    /// Get player setting values from overall game settings.
    func toPlayerSettings() -> PlayerSettings {
        PlayerSettings(
            showHelpers: self.showHelpers,
            checkMode: self.checkMode,
            testMode: self.testMode
        )
    }

    /// Get default game settings.
    static func `default`() -> Settings {
        Settings(
            boardSize: DEFAULT_BOARD_SIZE,
            autoplayMode: false,
            checkMode: false,
            showHelpers: true,
            showLog: false,
            testMode: false,
            useDefaults: false
        )
    }
}

/// Player settings.
public struct PlayerSettings: Equatable {
    var showHelpers: Bool
    var checkMode: Bool
    var testMode: Bool

    /// Get default player settings.
    static func `default`() -> PlayerSettings {
        PlayerSettings(
            showHelpers: true,
            checkMode: false,
            testMode: false
        )
    }
}

extension Settings: CustomStringConvertible {
    public var description: String {
        """
        Settings:
          board_size: \(self.boardSize)
          autoplay: \(self.autoplayMode)
          check_mode: \(self.checkMode)
          use_defaults: \(self.useDefaults)
          show_helpers: \(self.showHelpers)
          show_log: \(self.showLog)
          test_mode: \(self.testMode)
        """
    }
}
