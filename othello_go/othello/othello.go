//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

package othello

import (
	"fmt"
	"strings"
)

type Othello struct {
	boardSize    int
	board        Board
	gamesPlayed  int
	playerBlack  Player
	playerWhite  Player
	roundsPlayed int
}

func NewOthello(size int) Othello {
	return Othello{
		board:        NewBoard(size),
		boardSize:    size,
		playerBlack:  *BlackPlayer(),
		playerWhite:  *WhitePlayer(),
		roundsPlayed: 0,
		gamesPlayed:  0,
	}
}

func (o *Othello) Play() {
	for {
		o.initGame()
		o.gameLoop()
		o.printResult()
		if !o.getAnswer("\nWould you like to play again", "y", "n") {
			break
		}
	}
}

func (o *Othello) initGame() {
	if o.gamesPlayed > 0 {
		o.board = NewBoard(o.boardSize)
		o.playerBlack.Reset()
		o.playerWhite.Reset()
		o.roundsPlayed = 0
	}

	if o.getAnswer("Would you like to play against the computer", "y", "n") {
		if o.getAnswer("Would you like to play as black or white", "b", "w") {
			o.playerWhite.SetHuman(false)
		} else {
			o.playerBlack.SetHuman(false)
		}
	}
	fmt.Println("\nPlayers:")
	o.printStatus()
}

func (o *Othello) gameLoop() {
	for o.board.CanPlay() && (o.playerBlack.CanPlay || o.playerWhite.CanPlay) {
		o.roundsPlayed++
		fmt.Printf("\n=========== ROUND: %d ===========\n", o.roundsPlayed)
		o.playerBlack.PlayOneMove(&o.board)
		fmt.Println("--------------------------------")
		o.playerWhite.PlayOneMove(&o.board)
	}
}

func (o *Othello) getAnswer(text, yes, no string) bool {
	fmt.Printf("%s (%s/%s)? ", text, yes, no)
	var input string
	if _, err := fmt.Scanln(&input); err == nil {
		return strings.ToLower(strings.TrimSpace(input)) == yes
	}
	return false
}

func (o *Othello) printStatus() {
	fmt.Println(o.playerBlack)
	fmt.Println(o.playerWhite)
	fmt.Println(o.board)
}

func (o *Othello) printResult() {
	fmt.Println("\n================================")
	fmt.Println("The game is finished!")
	fmt.Println("\nResult:")
	o.printStatus()
	fmt.Println()

	winner := o.board.Result()
	if winner == Empty {
		fmt.Println("The game ended in a tie...")
	} else {
		fmt.Printf("The winner is %v!\n", winner)
	}
}

// GetBoardSize Ask and return the desired board size.
func GetBoardSize() int {
	fmt.Print("Choose board size (default is 8): ")
	var input int
	_, err := fmt.Scanf("%d", &input)
	if err != nil {
		PrintWarn("Invalid value, defaulting to 8...")
		return 8
	} else {
		return Clamp(input, 4, 10)
	}
}
