//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2024
//==========================================================

package othello

import (
	"errors"
	"fmt"
	mapset "github.com/deckarep/golang-set/v2"
	"github.com/logrusorgru/aurora/v4"
	"sort"
)

// Board Handles game board state and logic.
type Board struct {
	board        []Disk
	size         int
	emptySquares mapset.Set[Square]
	indices      []int
	directions   [8]Step
}

func NewBoard(size int) Board {
	board := initBoard(size)

	// Index list (0...size) to avoid repeating same range in loops.
	indices := make([]int, size)
	for i := range indices {
		indices[i] = i
	}

	// Keep track of empty squares on board to avoid checking already filled positions.
	emptySquares := mapset.NewSet[Square]()
	for y := range indices {
		for x := range indices {
			if board[y*size+x] == Empty {
				emptySquares.Append(Square{
					x,
					y,
				})
			}
		}
	}

	return Board{
		board:        board,
		size:         size,
		emptySquares: emptySquares,
		indices:      indices,
		directions: [8]Step{
			{X: -1, Y: -1},
			{X: -1, Y: 0},
			{X: -1, Y: 1},
			{X: 0, Y: -1},
			{X: 0, Y: 1},
			{X: 1, Y: -1},
			{X: 1, Y: 0},
			{X: 1, Y: 1},
		},
	}
}

// Initialize game board with starting disk positions.
func initBoard(size int) []Disk {
	board := make([]Disk, size*size)
	for i := range board {
		board[i] = Empty
	}
	// Set starting positions
	var row int
	if size%2 == 0 {
		row = (size - 1) / 2
	} else {
		row = (size-1)/2 - 1
	}
	col := size / 2
	board[row*size+row] = White
	board[row*size+col] = Black
	board[col*size+row] = Black
	board[col*size+col] = White
	return board
}

// CanPlay Return true if board contains empty squares.
func (b *Board) CanPlay() bool {
	return b.emptySquares.Cardinality() > 0
}

// PlaceDisk Update board for given disk placement.
func (b *Board) PlaceDisk(playerMove *Move) {
	start := playerMove.Square
	if b.getSquare(&start) != Empty {
		panic(fmt.Sprintf("Trying to place disk to an occupied square: %s!", start))
	}
	b.setSquare(&start, playerMove.Disk)
	b.emptySquares.Remove(start)
	for _, step := range playerMove.Directions {
		pos := start.Add(step)
		for b.getSquare(&pos) == playerMove.Disk.Opponent() {
			b.setSquare(&pos, playerMove.Disk)
			pos = pos.Add(step)
		}
	}
}

// PossibleMoves Returns a list of possible moves for the given disk colour.
func (b *Board) PossibleMoves(disk Disk) []Move {
	var moves []Move
	opposingDisk := disk.Opponent()
	for square := range b.emptySquares.Iter() {
		var value int
		var directions []Step
		for _, step := range b.directions {
			pos := square.Add(step)
			// Next square in this direction needs to be the opposing disk
			if b.getSquare(&pos) != opposingDisk {
				continue
			}
			var numSteps int
			for b.getSquare(&pos) == opposingDisk {
				numSteps++
				pos = pos.Add(step)
			}
			// Valid move only if a line of opposing disks ends in own disk
			if b.getSquare(&pos) == disk {
				value += numSteps
				directions = append(directions, step)
			}
		}
		if value > 0 {
			moves = append(moves, Move{
				Square:     square,
				Disk:       disk,
				Value:      value,
				Directions: directions,
			})
		}
	}
	if len(moves) > 0 {
		sort.Sort(MovesDescending(moves))
	}
	return moves
}

// printPossibleMoves Print board with available move coordinates and the resulting points gained.
func (b *Board) printPossibleMoves(moves []Move) {
	fmt.Println(aurora.Yellow(fmt.Sprintf("  Possible moves (%d):", len(moves))))
	// Convert board from Disk enums to strings
	formattedBoard := make([]string, len(b.board))
	for i, disk := range b.board {
		formattedBoard[i] = disk.BoardCharWithColor()
	}
	// Add possible moves to board
	for _, possibleMove := range moves {
		index := possibleMove.Square.Y*b.size + possibleMove.Square.X
		formattedBoard[index] = aurora.Yellow(fmt.Sprintf("%d", possibleMove.Value)).String()
		fmt.Printf("  %s\n", possibleMove)
	}
	// Print board with move positions
	fmt.Print("   ")
	for i := range b.indices {
		fmt.Printf(" %d", i)
	}
	for y := range b.indices {
		fmt.Printf("\n  %d", y)
		for x := range b.indices {
			fmt.Printf(" %s", formattedBoard[y*b.size+x])
		}
	}
	fmt.Println()
}

// PrintScore Print current score for both players.
func (b *Board) PrintScore() {
	black, white := b.playerScores()
	fmt.Println()
	fmt.Println(b)
	fmt.Printf("Score: %d | %d\n", aurora.Magenta(black), aurora.Cyan(white))
}

// Result Returns the winner disk color.
func (b *Board) Result() Disk {
	sum := b.score()
	switch {
	case sum > 0:
		return White
	case sum < 0:
		return Black
	default:
		return Empty
	}
}

func (b *Board) ToLogEntry() string {
	result := ""
	for _, disk := range b.board {
		result += disk.BoardChar()
	}
	return result
}

// Check that the given coordinates are valid (inside the board).
func (b *Board) checkCoordinates(x, y int) bool {
	return x >= 0 && x < b.size && y >= 0 && y < b.size
}

// Returns the state of the board (empty, white, black) at the given coordinates.
func (b *Board) get(x, y int) (Disk, error) {
	if b.checkCoordinates(x, y) {
		return b.board[y*b.size+x], nil
	}
	return Empty, errors.New("invalid coordinates")
}

// Returns the state of the board (empty, white, black) at the given square.
func (b *Board) getSquare(square *Square) Disk {
	if b.checkCoordinates(square.X, square.Y) {
		return b.board[square.Y*b.size+square.X]
	}
	// Square is out of bounds.
	// This probably should return a (Disk, error) instead but that makes using this quite cumbersome...
	return Empty
}

// Count and return the number of black and white disks.
func (b *Board) playerScores() (int, int) {
	var black, white int
	for _, disk := range b.board {
		switch disk {
		case Black:
			black++
		case White:
			white++
		}
	}
	return black, white
}

// Returns the total score.
// Positive value means more white disks and negative means more black disks.
func (b *Board) score() int {
	var sum int
	for _, disk := range b.board {
		sum += int(disk)
	}
	return sum
}

// Sets the given square to given value.
func (b *Board) setSquare(square *Square, disk Disk) {
	if !b.checkCoordinates(square.X, square.Y) {
		panic("Invalid coordinates")
	}
	b.board[square.Y*b.size+square.X] = disk
}

// Format game board to string
func (b *Board) String() string {
	text := " "
	// Horizontal indices
	for _, i := range b.indices {
		text += fmt.Sprintf(" %d", i)
	}
	for _, y := range b.indices {
		// Vertical index
		text += fmt.Sprintf("\n%d", y)
		// Row values
		for _, x := range b.indices {
			disk := b.board[y*b.size+x]
			text += fmt.Sprintf(" %s", disk.BoardCharWithColor())
		}
	}
	return text
}
