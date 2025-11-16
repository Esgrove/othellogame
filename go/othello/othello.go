//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2025
//==========================================================

package othello

import (
	"fmt"
	"strings"

	"github.com/logrusorgru/aurora/v4"
)

// Othello Gameplay loop and main logic.
type Othello struct {
	board        Board
	settings     Settings
	gamesPlayed  int
	playerBlack  Player
	playerWhite  Player
	roundsPlayed int
	gameLog      []string
}

// InitOthello Initialize Othello game.
func InitOthello(settings Settings) Othello {
	return Othello{
		board:        NewBoard(settings.BoardSize),
		settings:     settings,
		playerBlack:  *BlackPlayer(settings.ToPlayerSettings()),
		playerWhite:  *WhitePlayer(settings.ToPlayerSettings()),
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
		if o.settings.ShowLog {
			o.printLog()
		}
		if o.settings.AutoplayMode || !GetAnswer("Would you like to play again", "y", "n") {
			break
		}
	}
}

// Initialise game board and players for a new game.
func (o *Othello) initGame() {
	if o.gamesPlayed > 0 {
		o.board = NewBoard(o.settings.BoardSize)
		o.playerBlack.Reset()
		o.playerWhite.Reset()
		o.roundsPlayed = 0
		o.gameLog = []string{}
	}

	if o.settings.AutoplayMode {
		// Computer plays both
		o.playerBlack.SetHuman(false)
		o.playerWhite.SetHuman(false)
	} else if o.settings.UseDefaults {
		// Default: play as black against white computer player
		o.playerWhite.SetHuman(false)
	} else if GetAnswer("Would you like to play against the computer", "y", "n") {
		if GetAnswer("Would you like to play as black or white", "b", "w") {
			o.playerWhite.SetHuman(false)
		} else {
			o.playerBlack.SetHuman(false)
		}
	}
	if !o.settings.CheckMode {
		PrintBold("\nPlayers:")
		o.printStatus()
	}
}

// Keep making moves until both players can't make a move any more.
func (o *Othello) gameLoop() {
	players := []*Player{&o.playerBlack, &o.playerWhite}
	for o.board.CanPlay() && (o.playerBlack.CanPlay || o.playerWhite.CanPlay) {
		o.roundsPlayed++
		if !o.settings.CheckMode {
			PrintBold("\n=========== ROUND: %d ===========", o.roundsPlayed)
		}
		for _, player := range players {
			result := player.PlayOneMove(&o.board)
			if result != nil {
				o.gameLog = append(o.gameLog, fmt.Sprintf("%s;%s", *result, o.board.LogEntry()))
			}
			if !o.settings.CheckMode {
				fmt.Println("--------------------------------")
			}
		}
	}
	o.gamesPlayed++
	if !o.settings.CheckMode {
		PrintBold("\n================================")
		fmt.Println(aurora.Green("The game is finished!"))
	}
}

// Print ending status and winner info.
func (o *Othello) printLog() {
	formattedLog := ""
	for index, line := range o.gameLog {
		formattedLog += fmt.Sprintf("%02d: %s", index+1, line)
		if index < len(o.gameLog)-1 {
			formattedLog += "\n"
		}
	}
	if !o.settings.CheckMode {
		fmt.Println(aurora.Yellow("Game log:").Bold())
		fmt.Println(formattedLog)
	}
	hexHash := calculateSHA256(formattedLog)
	fmt.Println(hexHash)
}

// Print ending status and winner info.
func (o *Othello) printResult() {
	PrintBold("\nResult:")
	o.printStatus()
	fmt.Println()

	winner := o.board.Result()
	if winner == Empty {
		fmt.Print("The game ended in a tie...\n\n")
	} else {
		fmt.Printf("The winner is %s!\n\n", winner.DiskString())
	}
}

// Print current board and player info.
func (o *Othello) printStatus() {
	fmt.Println(o.playerBlack.String())
	fmt.Print(o.playerWhite.String() + "\n\n")
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
	fmt.Printf("Choose board size (default is %d): ", DefaultBoardSize)
	var input int
	_, err := fmt.Scanf("%d", &input)
	if err != nil {
		PrintWarn("Invalid size, defaulting to %d...", DefaultBoardSize)
		return DefaultBoardSize
	} else if input < MinBoardSize || input > MaxBoardSize {
		PrintWarn("Limiting board size to valid range %d...%d", MinBoardSize, MaxBoardSize)
	}
	return Clamp(input, MinBoardSize, MaxBoardSize)
}
