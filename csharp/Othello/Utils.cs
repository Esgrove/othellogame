//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2025
//==========================================================

using System.Security.Cryptography;
using System.Text;

namespace Othello {
    // Can't have a function without a class :(
    public static class Utils {
        /// Calculate SHA256 hash for the given string.
        public static string CalculateSha256(string input) {
            byte[] inputBytes = Encoding.UTF8.GetBytes(input);
            byte[] hashBytes = SHA256.HashData(inputBytes);

            StringBuilder builder = new();
            foreach (byte b in hashBytes) {
                builder.Append(b.ToString("x2", System.Globalization.CultureInfo.InvariantCulture));
            }

            return builder.ToString();
        }

        /// Return version info string
        public static string VersionInfo() {
            return $"{Version.VersionNumber} {Version.BuildTime} {Version.GitCommit} {Version.GitBranch}";
        }
    }
}
