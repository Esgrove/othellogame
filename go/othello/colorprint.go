//==========================================================
// Colorprint
// Interface for coloured terminal printing
// Akseli Lukkarila
// 2019-2026
//==========================================================

package othello

import (
	"fmt"
	"strings"
	"unicode"

	"github.com/logrusorgru/aurora/v4"
)

// PrintError Print error message with red colour.
// Supports optional Printf formatting.
func PrintError(message string, values ...any) {
	indent, text := splitLeadingWhitespace(fmt.Sprintf(message, values...))
	fmt.Println(aurora.Red(fmt.Sprintf("%sError: %s", indent, text)))
}

// PrintWarn Print warning message with yellow colour.
// Supports optional Printf formatting.
func PrintWarn(message string, values ...any) {
	indent, text := splitLeadingWhitespace(fmt.Sprintf(message, values...))
	fmt.Println(aurora.Yellow(fmt.Sprintf("%sWarning: %s", indent, text)))
}

// Split a string into the leading whitespace and the rest of the string.
func splitLeadingWhitespace(message string) (string, string) {
	text := strings.TrimLeftFunc(message, unicode.IsSpace)
	return message[:len(message)-len(text)], text
}
