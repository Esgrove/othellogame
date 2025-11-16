//==========================================================
// Settings
// Defines game and player settings.
// Akseli Lukkarila
// 2019-2025
//==========================================================

let MIN_BOARD_SIZE: Int = 4
let MAX_BOARD_SIZE: Int = 10
let DEFAULT_BOARD_SIZE: Int = 8

/// Game settings.
struct Settings: CustomStringConvertible {
    var boardSize: Int
    var autoplayMode: Bool
    var checkMode: Bool
    var showHelpers: Bool
    var showLog: Bool
    var testMode: Bool
    var useDefaults: Bool

    // Get player setting values from overall game settings.
    func toPlayerSettings() -> PlayerSettings {
        PlayerSettings(
            checkMode: self.checkMode,
            showHelpers: self.showHelpers,
            testMode: self.testMode
        )
    }

    var description: String {
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
    var checkMode: Bool
    var showHelpers: Bool
    var testMode: Bool
}
