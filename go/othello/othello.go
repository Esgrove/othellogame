//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

package othello

import (
	"fmt"
	"strings"
)

// Othello Gameplay loop and main logic.
type Othello struct {
	board        Board
	settings     Settings
	playerBlack  Player
	playerWhite  Player
	gameLog      []string
	gamesPlayed  int
	roundsPlayed int
}

// NewOthello Initialize Othello game.
func NewOthello(settings Settings) Othello {
	return Othello{
		board:        NewBoard(settings.BoardSize),
		settings:     settings,
		playerBlack:  *BlackPlayer(settings.ToPlayerSettings()),
		playerWhite:  *WhitePlayer(settings.ToPlayerSettings()),
		gameLog:      []string{},
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
		if o.settings.AutoplayMode || !getAnswer("Would you like to play again", "y", "n") {
			break
		}
	}
}

// Initialize game board and players for a new game.
func (o *Othello) initGame() {
	// Re-use existing objects instead of initializing new ones
	if o.gamesPlayed > 0 {
		o.board = NewBoard(o.settings.BoardSize)
		o.playerBlack.Reset()
		o.playerWhite.Reset()
		o.roundsPlayed = 0
		o.gameLog = []string{}
	}
	if o.settings.AutoplayMode {
		// Computer plays both
		o.playerBlack.SetComputer()
		o.playerWhite.SetComputer()
	} else if o.settings.UseDefaults {
		// Default: play as black against white computer player
		o.playerWhite.SetComputer()
	} else if getAnswer("Would you like to play against the computer", "y", "n") {
		if getAnswer("Would you like to play as black or white", "b", "w") {
			o.playerWhite.SetComputer()
		} else {
			o.playerBlack.SetComputer()
		}
	}
	if !o.settings.CheckMode {
		PrintBold("\nPlayers:")
		o.printStatus()
	}
}

// Keep making moves until both players can't make a move any more.
func (o *Othello) gameLoop() {
	for o.board.CanPlay() && (o.playerBlack.CanPlay || o.playerWhite.CanPlay) {
		o.roundsPlayed++
		o.printRoundHeader()
		for _, player := range []*Player{&o.playerBlack, &o.playerWhite} {
			if result := player.PlayOneMove(&o.board); result != nil {
				o.gameLog = append(o.gameLog, fmt.Sprintf("%s;%s", *result, o.board.LogEntry()))
			}
			if !o.settings.CheckMode {
				fmt.Println("--------------------------------")
			}
		}
	}
	o.gamesPlayed++
	o.printGameEndFooter()
}

// Format game log with line numbers for each move.
func (o *Othello) formatGameLog() string {
	lines := make([]string, len(o.gameLog))
	for index, line := range o.gameLog {
		lines[index] = fmt.Sprintf("%02d: %s", index+1, line)
	}
	return strings.Join(lines, "\n")
}

// Print header for the current round.
func (o *Othello) printRoundHeader() {
	if !o.settings.CheckMode {
		PrintBold(fmt.Sprintf("\n=========== ROUND: %d ===========", o.roundsPlayed))
	}
}

// Print footer after the game has ended.
func (o *Othello) printGameEndFooter() {
	if !o.settings.CheckMode {
		PrintBold("\n================================")
		PrintGreenBold("The game is finished!\n")
	}
}

// Print game log which shows all moves made and the game board state after each move.
func (o *Othello) printLog() {
	formattedLog := o.formatGameLog()
	if !o.settings.CheckMode {
		PrintYellowBold("Game log:")
		fmt.Println(formattedLog)
	}
	hexHash := CalculateSHA256(formattedLog)
	fmt.Println(hexHash)
}

// Print ending status and winner info.
func (o *Othello) printResult() {
	PrintBold("Result:")
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
	fmt.Printf("%s\n\n", o.playerWhite.String())
	fmt.Println(o.board.String())
}

// Ask a question with two options, and return bool from user answer.
func getAnswer(question, yes, no string) bool {
	fmt.Printf("%s (%s/%s)? ", question, yes, no)
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
	if _, err := fmt.Scanf("%d", &input); err == nil {
		if input < MinBoardSize || input > MaxBoardSize {
			PrintYellow(fmt.Sprintf(
				"Limiting board size to valid range %d..%d", MinBoardSize, MaxBoardSize,
			))
		}
		return Clamp(input, MinBoardSize, MaxBoardSize)
	}
	PrintWarn("Invalid size, defaulting to %d...", DefaultBoardSize)
	return DefaultBoardSize
}
