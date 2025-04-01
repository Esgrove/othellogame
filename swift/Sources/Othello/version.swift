import VersionInfo

/// Return formatted build version information.
func versionInfo() -> String {
    return "\(VersionInfo.version) \(VersionInfo.buildTime) \(VersionInfo.gitBranch) \(VersionInfo.gitHash)"
}
