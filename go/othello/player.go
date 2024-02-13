//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2024
//==========================================================

package othello

import (
	"fmt"
	"github.com/logrusorgru/aurora/v4"
	"math/rand"
	"time"
)

// Player Defines one player (human or computer).
type Player struct {
	CanPlay      bool
	color        Disk
	human        bool
	roundsPlayed int
	settings     PlayerSettings
}

func NewPlayer(color Disk, settings PlayerSettings) *Player {
	return &Player{
		color:    color,
		human:    true,
		CanPlay:  true,
		settings: settings,
	}
}

func BlackPlayer(settings PlayerSettings) *Player {
	return NewPlayer(Black, settings)
}

func WhitePlayer(settings PlayerSettings) *Player {
	return NewPlayer(White, settings)
}

// PlayOneMove Play one round as this player.
func (p *Player) PlayOneMove(board *Board) *string {
	fmt.Printf("Turn: %s\n", p.color.DiskString())
	moves := board.PossibleMoves(p.color)
	if len(moves) > 0 {
		p.CanPlay = true
		if p.human && p.settings.ShowHelpers {
			board.printPossibleMoves(moves)
		}
		var chosenMove Move
		if p.human {
			chosenMove = p.getHumanMove(moves)
		} else {
			chosenMove = p.getComputerMove(moves)
		}
		board.PlaceDisk(&chosenMove)
		board.PrintScore()
		p.roundsPlayed++
		if !p.settings.TestMode {
			time.Sleep(time.Second)
		}
		logEntry := chosenMove.LogEntry()
		return &logEntry
	}
	p.CanPlay = false
	fmt.Println("  No moves available...")
	return nil
}

// Return move chosen by computer.
func (p *Player) getComputerMove(moves []Move) Move {
	fmt.Println("  Computer plays...")
	var chosenMove Move
	if p.settings.TestMode {
		chosenMove = moves[0]
	} else {
		// Wait a bit and pick a random move
		time.Sleep(time.Millisecond * time.Duration(rand.Intn(1000)+1000))
		chosenMove = moves[rand.Intn(len(moves))]
	}

	fmt.Printf("  %s -> %d\n", chosenMove.Square, chosenMove.Value)
	return chosenMove
}

// Return move chosen by a human player.
func (p *Player) getHumanMove(moves []Move) Move {
	for {
		square := p.getSquare()
		// Check that the chosen square is actually one of the possible moves
		for _, validMove := range moves {
			if validMove.Square == square {
				return validMove
			}
		}
		fmt.Printf("  Can't place a %s disk in square %s!\n", p.color.DiskString(), square)
	}
}

// Ask human player for square coordinates.
func (p *Player) getSquare() Square {
	for {
		fmt.Print("  Give disk position (x,y): ")
		var x, y int
		_, err := fmt.Scanf("%d,%d", &x, &y)
		if err == nil && x >= 0 && y >= 0 {
			return Square{x, y}
		}
		fmt.Print(aurora.Red("  Give coordinates in the form 'x,y'\n\n"))
	}
}

// Reset player status for a new game.
func (p *Player) Reset() {
	p.CanPlay = true
	p.roundsPlayed = 0
}

// SetHuman Set the player as human or computer controlled.
func (p *Player) SetHuman(isHuman bool) {
	p.human = isHuman
}

func (p *Player) String() string {
	return fmt.Sprintf("%s | %s | Moves: %d", p.color.DiskString(), p.typeString(), p.roundsPlayed)
}

// Return player type description string.
func (p *Player) typeString() string {
	if p.human {
		return "Human   "
	}
	return "Computer"
}
