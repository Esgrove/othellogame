//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2025
//==========================================================

package othello

import (
	"fmt"

	"github.com/logrusorgru/aurora/v4"
)

// Print error message with red colour.
// Supports optional Printf formatting.
func PrintError(message string, values ...any) {
	fmt.Println(aurora.Red(fmt.Sprintf("ERROR: "+message, values...)))
}

// Print warning message with yellow colour.
// Supports optional Printf formatting.
func PrintWarn(message string, values ...any) {
	fmt.Println(aurora.Yellow(fmt.Sprintf("WARNING: "+message, values...)))
}

// Print bold message with white colour.
// Supports optional Printf formatting.
func PrintBold(message string, values ...any) {
	fmt.Println(aurora.Bold(fmt.Sprintf(message, values...)))
}
