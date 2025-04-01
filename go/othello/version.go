//==========================================================
// Version
// Version information
// Akseli Lukkarila
// 2019-2025
//==========================================================

package othello

import (
	"fmt"
)

// Set at build time
var GitBranch string
var GitCommit string
var BuildTime string
var VersionNumber string

// Returns formatted build version info string.
func VersionInfo() string {
	return fmt.Sprintf(
		"%s %s %s %s",
		VersionNumber, BuildTime, GitBranch, GitCommit,
	)
}
