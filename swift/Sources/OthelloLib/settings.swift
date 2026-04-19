//==========================================================
// Settings
// Defines game and player settings.
// Akseli Lukkarila
// 2019-2026
//==========================================================

public let MIN_BOARD_SIZE: Int = 4
public let MAX_BOARD_SIZE: Int = 10
public let DEFAULT_BOARD_SIZE: Int = 8

/// Game settings.
public struct Settings: CustomStringConvertible {
    var boardSize: Int
    var autoplayMode: Bool
    var checkMode: Bool
    var showHelpers: Bool
    var showLog: Bool
    var testMode: Bool
    var useDefaults: Bool

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

    public var description: String {
        """
        Settings:
          boardSize: \(self.boardSize)
          autoplayMode: \(self.autoplayMode)
          checkMode: \(self.checkMode)
          showHelpers: \(self.showHelpers)
          showLog: \(self.showLog)
          testMode: \(self.testMode)
          useDefaults: \(self.useDefaults)
        """
    }
}

/// Player settings.
struct PlayerSettings {
    var showHelpers: Bool
    var checkMode: Bool
    var testMode: Bool
}
