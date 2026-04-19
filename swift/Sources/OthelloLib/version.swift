import VersionInfo

/// Return formatted build version information.
func versionInfo() -> String {
    "\(VersionInfo.versionNumber) \(VersionInfo.buildTime) \(VersionInfo.gitBranch) \(VersionInfo.gitCommit)"
}
