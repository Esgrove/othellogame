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
	"github.com/logrusorgru/aurora/v4"
	"strings"
)

// Othello Gameplay loop and main logic.
type Othello struct {
	boardSize    int
	board        Board
	gamesPlayed  int
	playerBlack  Player
	playerWhite  Player
	roundsPlayed int
}

// InitOthello Initialize Othello game.
func InitOthello(size int) Othello {
	return Othello{
		board:        NewBoard(size),
		boardSize:    size,
		playerBlack:  *BlackPlayer(),
		playerWhite:  *WhitePlayer(),
		roundsPlayed: 0,
		gamesPlayed:  0,
	}
}

// Play one full game of Othello.
func (o *Othello) Play() {
	for {
		o.initGame()
		o.gameLoop()
		o.printResult()
		if !GetAnswer("\nWould you like to play again", "y", "n") {
			break
		}
	}
}

// Initialize game board and players for a new game.
func (o *Othello) initGame() {
	if o.gamesPlayed > 0 {
		o.board = NewBoard(o.boardSize)
		o.playerBlack.Reset()
		o.playerWhite.Reset()
		o.roundsPlayed = 0
	}

	if GetAnswer("Would you like to play against the computer", "y", "n") {
		if GetAnswer("Would you like to play as black or white", "b", "w") {
			o.playerWhite.SetHuman(false)
		} else {
			o.playerBlack.SetHuman(false)
		}
	}
	PrintBold("\nPlayers:")
	o.printStatus()
}

// Keep making moves until both players can't make a move any more.
func (o *Othello) gameLoop() {
	for o.board.CanPlay() && (o.playerBlack.CanPlay || o.playerWhite.CanPlay) {
		o.roundsPlayed++
		PrintBold("\n=========== ROUND: %d ===========", o.roundsPlayed)
		o.playerBlack.PlayOneMove(&o.board)
		fmt.Println("--------------------------------")
		o.playerWhite.PlayOneMove(&o.board)
	}
}

// Print ending status and winner info.
func (o *Othello) printResult() {
	PrintBold("\n================================")
	fmt.Println(aurora.Green("The game is finished!"))
	PrintBold("\nResult:")
	o.printStatus()
	fmt.Println()

	winner := o.board.Result()
	if winner == Empty {
		fmt.Println("The game ended in a tie...")
	} else {
		fmt.Printf("The winner is %s!\n", winner.DiskString())
	}
}

// Print current board and player info.
func (o *Othello) printStatus() {
	fmt.Println(o.playerBlack.String())
	fmt.Println(o.playerWhite.String())
	fmt.Println(o.board.String())
}

// GetAnswer Ask a question with two options, and return bool from user answer.
func GetAnswer(text, yes, no string) bool {
	fmt.Printf("%s (%s/%s)? ", text, yes, no)
	var input string
	if _, err := fmt.Scanln(&input); err == nil {
		return strings.ToLower(strings.TrimSpace(input)) == yes
	}
	return false
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
