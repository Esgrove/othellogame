//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2026
//==========================================================

package othello

import (
	"fmt"
	"math/rand"
	"time"
)

// Player Defines one player that can be either human or computer controlled.
type Player struct {
	CanPlay      bool
	disk         Disk
	playerType   PlayerType
	roundsPlayed int
	settings     PlayerSettings
}

// PlayerType Player can be controlled either by a human or computer.
type PlayerType int

const (
	Human    PlayerType = 0
	Computer PlayerType = 1
)

// NewPlayer Initialize new player for the given disk colour.
func NewPlayer(disk Disk, settings PlayerSettings) *Player {
	return &Player{
		CanPlay:    true,
		disk:       disk,
		playerType: Human,
		settings:   settings,
	}
}

// BlackPlayer Shorthand to initialize a new player for black disks.
//
// Note: deliberately named `BlackPlayer` instead of `Black`
// to avoid a name collision with the package-level `Black` disk constant.
func BlackPlayer(settings PlayerSettings) *Player {
	return NewPlayer(Black, settings)
}

// WhitePlayer Shorthand to initialize a new player for white disks.
//
// Note: deliberately named `WhitePlayer` instead of `White`
// to avoid a name collision with the package-level `White` disk constant.
func WhitePlayer(settings PlayerSettings) *Player {
	return NewPlayer(White, settings)
}

// PlayOneMove Play one round as this player.
func (p *Player) PlayOneMove(board *Board) *string {
	if !p.settings.CheckMode {
		fmt.Printf("Turn: %s\n", p.disk.DiskString())
	}
	moves := board.PossibleMoves(p.disk)
	if len(moves) == 0 {
		p.CanPlay = false
		if !p.settings.CheckMode {
			PrintYellow("  No moves available...")
		}
		return nil
	}

	p.CanPlay = true
	if p.Human() && p.settings.ShowHelpers && !p.settings.CheckMode {
		board.PrintPossibleMoves(moves)
	}
	var chosenMove Move
	if p.Human() {
		chosenMove = p.getHumanMove(moves)
	} else {
		chosenMove = p.getComputerMove(moves)
	}
	board.PlaceDisk(&chosenMove)
	if !p.settings.CheckMode {
		board.PrintScore()
	}
	p.roundsPlayed++
	if !p.settings.TestMode {
		time.Sleep(time.Second)
	}
	logEntry := chosenMove.LogEntry()

	return &logEntry
}

// Reset player status for a new game.
func (p *Player) Reset() {
	p.CanPlay = true
	p.roundsPlayed = 0
}

// Human Returns true if player is controlled by a human player.
func (p *Player) Human() bool {
	return p.playerType.Human()
}

// IsComputer Returns true if player is controlled by computer.
//
// Note: deliberately named `IsComputer` instead of `Computer`
// to avoid confusion with the package-level `Computer` player type constant.
func (p *Player) IsComputer() bool {
	return p.playerType.Computer()
}

// SetPlayerType Set the player as human or computer controlled.
func (p *Player) SetPlayerType(playerType PlayerType) {
	p.playerType = playerType
}

// SetHuman Set the player as human controlled.
func (p *Player) SetHuman() {
	p.SetPlayerType(Human)
}

// SetComputer Set the player as computer controlled.
func (p *Player) SetComputer() {
	p.SetPlayerType(Computer)
}

// Return move chosen by computer.
func (p *Player) getComputerMove(moves []Move) Move {
	if !p.settings.CheckMode {
		fmt.Println("  Computer plays...")
	}
	var chosenMove Move
	if p.settings.TestMode {
		chosenMove = moves[0]
	} else {
		// Wait a bit and pick a random move
		time.Sleep(time.Millisecond * time.Duration(rand.Intn(1000)+1000))
		chosenMove = moves[rand.Intn(len(moves))]
	}
	if !p.settings.CheckMode {
		fmt.Printf("  %s -> %d\n", chosenMove.Square, chosenMove.Value)
	}
	return chosenMove
}

// Return move chosen by a human player.
func (p *Player) getHumanMove(moves []Move) Move {
	for {
		square := getSquare()
		// Check that the chosen square is actually one of the possible moves
		for _, validMove := range moves {
			if validMove.Square == square {
				return validMove
			}
		}
		PrintError("  Can't place a %s disk in square %s!", p.disk.DiskString(), square)
	}
}

// Ask human player for square coordinates.
func getSquare() Square {
	for {
		fmt.Print("  Give disk position (x,y): ")
		var x, y int
		if _, err := fmt.Scanf("%d,%d", &x, &y); err == nil && x >= 0 && y >= 0 {
			return Square{X: x, Y: y}
		}
		PrintError("  Give coordinates in the form 'x,y'!")
	}
}

// Return player type description string.
func (p *Player) typeString() string {
	return p.playerType.String()
}

// Human Check if the player is controlled by a human.
func (t PlayerType) Human() bool {
	return t == Human
}

// Computer Check if the player is controlled by a computer.
func (t PlayerType) Computer() bool {
	return t == Computer
}

// String Return formatted player info string.
func (p *Player) String() string {
	return fmt.Sprintf("%s | %s | Moves: %d", p.disk.DiskString(), p.typeString(), p.roundsPlayed)
}

// String Return player type description string.
func (t PlayerType) String() string {
	if t == Human {
		return "Human   "
	}
	return "Computer"
}
