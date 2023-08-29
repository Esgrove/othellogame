//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

package main

import (
	"fmt"
	"os"
	"othello_go/othello"
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
		// Handle 'help' and 'version' arguments
		if slices.Contains(args, "--help") || slices.Contains(args, "-h") {
			fmt.Println(othello.VersionInfo())
			fmt.Println("\nUSAGE: othello_go [board size]")
			fmt.Println("Optional arguments:")
			fmt.Println("    -h | --help          Print usage and exit")
			fmt.Println("    -v | --version       Print version info and exit")
			os.Exit(1)
		}
		if slices.Contains(args, "--version") || slices.Contains(args, "-v") {
			fmt.Println(othello.VersionInfo())
			os.Exit(0)
		}
		// Try to read board size from command line args
		size, err := strconv.Atoi(args[0])
		if err != nil {
			othello.PrintError("Invalid board size")
			boardSize = othello.GetBoardSize()
		} else if size < othello.MinBoardSize || size > othello.MaxBoardSize {
			othello.PrintError("Unsupported board size: %d", size)
			boardSize = othello.GetBoardSize()
		} else {
			boardSize = size
			fmt.Printf("Using board size: %d\n", boardSize)
		}
	} else {
		// Otherwise ask user for board size
		boardSize = othello.GetBoardSize()
	}

	game := othello.InitOthello(boardSize)
	game.Play()
}
