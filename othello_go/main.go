package main

import (
	"fmt"
	// https://github.com/logrusorgru/aurora
	"github.com/logrusorgru/aurora/v4"
)

func main() {
	fmt.Println(aurora.Green("OTHELLO GAME - GO").Bold())
}
