//==========================================================
// Settings
// Defines game and player settings.
// Akseli Lukkarila
// 2019-2025
//==========================================================

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
        return PlayerSettings(
            checkMode: self.checkMode,
            showHelpers: self.showHelpers,
            testMode: self.testMode
        )
    }

    var description: String {
        return """
        Settings:
          boardSize: \(boardSize)
          autoplayMode: \(autoplayMode)
          checkMode: \(checkMode)
          showHelpers: \(showHelpers)
          showLog: \(showLog)
          testMode: \(testMode)
          useDefaults: \(useDefaults)
        """
    }
}

/// Player settings.
struct PlayerSettings {
    var checkMode: Bool
    var showHelpers: Bool
    var testMode: Bool
}
