//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2025
//==========================================================

import ColorizeSwift
import Crypto
import Foundation

import BuildInfo

let MIN_BOARD_SIZE: Int = 4
let MAX_BOARD_SIZE: Int = 10
let DEFAULT_BOARD_SIZE: Int = 8

/// Calculate SHA256 hash for the given string.
func calculateSHA256(_ text: String) -> String {
    let data = text.data(using: .utf8)!
    let hash = SHA256.hash(data: data)

    return hash.map { String(format: "%02x", $0) }.joined()
}

/// Return formatted build version information.
func versionInfo() -> String {
    let versionNumber = VERSION
    let buildTime = BUILD_TIME
    let gitHash = GIT_HASH
    let gitBranch = GIT_BRANCH
    return "\(versionNumber) \(buildTime) \(gitBranch) \(gitHash)"
}

extension Comparable {
    func clamp(min: Self, max: Self) -> Self {
        return Swift.max(min, Swift.min(self, max))
    }
}
