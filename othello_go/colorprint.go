package main

import (
	"fmt"
	"github.com/logrusorgru/aurora/v4"
)

// Print error message with red colour.
// Supports optional Printf formatting.
func printError(message string, values ...any) {
	fmt.Println(aurora.Red(fmt.Sprintf("ERROR: "+message, values...)).Bold())
}
