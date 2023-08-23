//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

package main

import "fmt"

type Othello struct {
	boardSize    int
	board        Board
	gamesPlayed  int
	playerBlack  Player
	playerWhite  Player
	roundsPlayed int
}

// Ask and return the desired board size.
func getBoardSize() int {
	fmt.Print("Choose board size (default is 8): ")
	var input int
	_, err := fmt.Scanf("%d", &input)
	if err != nil {
		printWarn("Invalid value, defaulting to 8...")
		return 8
	} else {
		return clamp(input, 4, 10)
	}
}
