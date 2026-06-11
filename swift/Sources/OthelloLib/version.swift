//==========================================================
// Version
// Version information
// Akseli Lukkarila
// 2019-2026
//==========================================================

import VersionInfo

/// Return version info string
public func versionInfo() -> String {
    "Othello Swift \(VersionInfo.versionNumber) \(VersionInfo.buildTime) \(VersionInfo.gitBranch) \(VersionInfo.gitCommit)"
}
