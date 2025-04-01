import VersionInfo

/// Return formatted build version information.
func versionInfo() -> String {
    return "\(VersionInfo.versionNumber) \(VersionInfo.buildTime) \(VersionInfo.gitBranch) \(VersionInfo.gitCommit)"
}
