//==========================================================
// Version
// Version information
// Akseli Lukkarila
// 2019-2025
//==========================================================

package othello

import (
	"fmt"
	"runtime/debug"
)

// Set at build time
var GitBranch string
var GitHash string
var Timestamp string
var VersionNumber string

// Returns formatted build version info string.
func VersionInfo() string {
	if info, ok := debug.ReadBuildInfo(); ok {
		goVersion := info.GoVersion
		arch := "unknown"
		for _, setting := range info.Settings {
			if setting.Key == "GOARCH" {
				arch = setting.Value
			}
		}
		return fmt.Sprintf(
			"%s %s %s %s %s %s",
			VersionNumber, Timestamp, GitBranch, GitHash, goVersion, arch,
		)
	}
	return fmt.Sprintf(
		"%s %s %s %s",
		VersionNumber, Timestamp, GitBranch, GitHash,
	)
}
