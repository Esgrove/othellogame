//==========================================================
// Version
// Version information
// Akseli Lukkarila
// 2019-2026
//==========================================================

package othello

// Variables set at build time.

var BuildTime string
var GitBranch string
var GitCommit string
var VersionNumber string
var VersionString string

// VersionInfo Return version info string.
func VersionInfo() string {
	return VersionString
}
