//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2025
//==========================================================

import ColorizeSwift
import Crypto
import Foundation

/// Calculate SHA256 hash for the given string.
func calculateSHA256(_ text: String) -> String {
    let data = text.data(using: .utf8)!
    let hash = SHA256.hash(data: data)

    return hash.map { String(format: "%02x", $0) }.joined()
}

extension Comparable {
    func clamp(min: Self, max: Self) -> Self {
        return Swift.max(min, Swift.min(self, max))
    }
}
