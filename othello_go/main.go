package main

import (
	"fmt"
	"os"
	"slices"
	"strconv"

	// https://github.com/logrusorgru/aurora
	"github.com/logrusorgru/aurora/v4"
)

func main() {
	fmt.Println(aurora.Green("OTHELLO GAME - GO").Bold())
	args := os.Args[1:]
	if slices.Contains(args, "--help") || slices.Contains(args, "-h") {
		fmt.Println(versionInfo())
		fmt.Println("USAGE: othello_go [board size]")
		fmt.Println("\nOptional arguments:")
		fmt.Println("    -h | --help          Print usage and exit")
		fmt.Println("    -v | --version       Print version info and exit")
		os.Exit(1)
	}
	if slices.Contains(args, "--version") || slices.Contains(args, "-v") {
		fmt.Println(versionInfo())
		os.Exit(0)
	}
	boardSize, err := strconv.Atoi(args[0])
	if err != nil {
		fmt.Println(aurora.Red("Invalid board size"))
	} else if boardSize < 4 || boardSize > 16 {
		fmt.Println(aurora.Red(fmt.Sprintf("Unsupported board size: %d", boardSize)))
	}

	fmt.Printf("Using board size: %d\n", boardSize)
}
