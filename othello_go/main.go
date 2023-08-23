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
	var boardSize int
	if len(args) > 0 {
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
		size, err := strconv.Atoi(args[0])
		if err != nil {
			printError("Invalid board size")
			boardSize = getBoardSize()
		} else if size < 4 || size > 16 {
			printError("Unsupported board size: %d", size)
			boardSize = getBoardSize()
		} else {
			boardSize = size
			fmt.Printf("Using board size: %d\n", boardSize)
		}
	} else {
		boardSize = getBoardSize()
	}

	fmt.Println(aurora.Green(fmt.Sprintf("Board size: %d\n", boardSize)))
}
