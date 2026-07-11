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

// PrintBold Print bold text.
func PrintBold(text string) {
	fmt.Println(aurora.Bold(text))
}

// PrintGreen Print text in green.
func PrintGreen(text string) {
	fmt.Println(aurora.Green(text))
}

// PrintGreenBold Print bold text in green.
func PrintGreenBold(text string) {
	fmt.Println(aurora.Green(text).Bold())
}

// PrintYellow Print text in yellow.
func PrintYellow(text string) {
	fmt.Println(aurora.Yellow(text))
}

// PrintYellowBold Print bold text in yellow.
func PrintYellowBold(text string) {
	fmt.Println(aurora.Yellow(text).Bold())
}

// PrintRed Print text in red.
func PrintRed(text string) {
	fmt.Println(aurora.Red(text))
}

// PrintRedBold Print bold text in red.
func PrintRedBold(text string) {
	fmt.Println(aurora.Red(text).Bold())
}

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
