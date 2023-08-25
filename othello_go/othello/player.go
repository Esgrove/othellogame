//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2023
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
	showHelpers  bool
}

func NewPlayer(color Disk) *Player {
	return &Player{
		color:       color,
		human:       true,
		CanPlay:     true,
		showHelpers: true,
	}
}

func BlackPlayer() *Player {
	return NewPlayer(Black)
}

func WhitePlayer() *Player {
	return NewPlayer(White)
}

// PlayOneMove Play one round as this player.
func (p *Player) PlayOneMove(board *Board) {
	fmt.Printf("Turn: %s\n", p.color.DiskString())
	moves := board.PossibleMoves(p.color)
	if len(moves) > 0 {
		p.CanPlay = true
		if p.human && p.showHelpers {
			board.PrintMoves(moves)
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
		time.Sleep(time.Second)
	} else {
		p.CanPlay = false
		fmt.Print("  No moves available...\n\n")
	}
}

// Return move chosen by computer.
func (p *Player) getComputerMove(moves []Move) Move {
	fmt.Println("  Computer plays...")

	time.Sleep(time.Millisecond * time.Duration(rand.Intn(1000)+1000))

	chosenMove := moves[rand.Intn(len(moves))]
	fmt.Printf("  -> %s\n", chosenMove.Square)
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
		if err != nil {
			fmt.Println("Input failed")
			continue
		}
		if x >= 0 && y >= 0 {
			return Square{X: x, Y: y}
		}
		fmt.Print(aurora.Red("  Give coordinates in the form 'x,y'\n\n"))
	}
}

// SetHuman Set the player as human or computer controlled
func (p *Player) SetHuman(isHuman bool) {
	p.human = isHuman
}

// Reset player status for a new game.
func (p *Player) Reset() {
	p.CanPlay = true
	p.roundsPlayed = 0
}

func (p *Player) String() string {
	humanStr := "Computer"
	if p.human {
		humanStr = "Human   "
	}
	return fmt.Sprintf("%s | %s | %d", p.color.DiskString(), humanStr, p.roundsPlayed)
}
