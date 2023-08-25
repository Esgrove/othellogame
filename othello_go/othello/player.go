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

func (p *Player) PlayOneMove(board *Board) {
	fmt.Printf("Turn: %v\n", p.color)
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
		fmt.Println("  No moves available...\n")
	}
}

func (p *Player) getComputerMove(moves []Move) Move {
	fmt.Println("  Computer plays...")

	time.Sleep(time.Millisecond * time.Duration(rand.Intn(1000)+1000))

	chosenMove := moves[rand.Intn(len(moves))]
	fmt.Printf("  -> %v\n", chosenMove.Square)
	return chosenMove
}

func (p *Player) getHumanMove(moves []Move) Move {
	for {
		square := p.getSquare()
		for _, validMove := range moves {
			if validMove.Square == square {
				return validMove
			}
		}
		fmt.Printf("  Can't place a %v disk in square %v!\n", p.color, square)
	}
}

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

func (p *Player) SetHuman(isHuman bool) {
	p.human = isHuman
}

func (p *Player) Reset() {
	p.CanPlay = true
	p.roundsPlayed = 0
}

func (p *Player) String() string {
	humanStr := "Computer"
	if p.human {
		humanStr = "Human   "
	}
	return fmt.Sprintf("%v | %s | %d", p.color, humanStr, p.roundsPlayed)
}
