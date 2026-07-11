//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2026
//==========================================================

package othello

import (
	"fmt"
	"sort"
	"strconv"
	"strings"

	mapset "github.com/deckarep/golang-set/v2"
	"github.com/logrusorgru/aurora/v4"
)

const (
	up    = 1
	down  = -1
	left  = -1
	right = 1
	still = 0
)

// All possible step directions for a square on the board.
var stepDirections = [8]Step{
	{X: down, Y: left},
	{X: down, Y: right},
	{X: down, Y: still},
	{X: still, Y: left},
	{X: still, Y: right},
	{X: up, Y: left},
	{X: up, Y: right},
	{X: up, Y: still},
}

// Board Handles game board state and logic.
type Board struct {
	board        []Disk
	emptySquares mapset.Set[Square]
	indices      []int
	size         int
}

// NewBoard Initialize a new game board with the given size.
func NewBoard(size int) Board {
	board := initBoard(size)

	// Index list (0...size) to avoid repeating same range in loops.
	indices := make([]int, size)
	for i := range indices {
		indices[i] = i
	}

	// Keep track of empty squares on board to avoid checking already filled positions.
	emptySquares := initEmptySquares(size, board)

	return Board{
		board:        board,
		emptySquares: emptySquares,
		indices:      indices,
		size:         size,
	}
}

// CanPlay Return true if board contains empty squares.
func (b *Board) CanPlay() bool {
	return b.emptySquares.Cardinality() > 0
}

// PlaceDisk Update board for given disk placement.
func (b *Board) PlaceDisk(chosenMove *Move) {
	start := chosenMove.Square
	disk, ok := b.getSquare(&start)
	if !ok {
		panic(fmt.Sprintf("Invalid coordinates: %s", start))
	}
	if disk != Empty {
		panic(fmt.Sprintf("Trying to place disk to an occupied square: %s!", start))
	}
	b.setSquare(&start, chosenMove.Disk)
	b.emptySquares.Remove(start)
	for _, square := range chosenMove.AffectedSquares() {
		b.setSquare(&square, chosenMove.Disk)
	}
}

// PossibleMoves Returns a list of possible moves for the given player.
func (b *Board) PossibleMoves(disk Disk) []Move {
	var moves []Move
	opposingDisk := disk.Opponent()
	for square := range b.emptySquares.Iter() {
		var value int
		var directions []Direction
		for _, step := range stepDirections {
			pos := square.Add(step)
			// Next square in this direction needs to be the opposing disk
			squareDisk, _ := b.getSquare(&pos)
			if squareDisk != opposingDisk {
				continue
			}
			var numSteps int
			// Keep stepping over opponents disks
			for squareDisk == opposingDisk {
				numSteps++
				pos = pos.Add(step)
				squareDisk, _ = b.getSquare(&pos)
			}
			// Valid move only if a line of opposing disks ends with own disk
			if squareDisk == disk {
				directions = append(directions, Direction{step, numSteps})
				value += numSteps
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

// PrintPossibleMoves Print board with available move coordinates and the resulting points gained.
func (b *Board) PrintPossibleMoves(moves []Move) {
	fmt.Println(aurora.Yellow(fmt.Sprintf("  Possible moves (%d):", len(moves))))
	// Convert board from Disk enums to strings
	formattedBoard := make([]string, len(b.board))
	for i, disk := range b.board {
		formattedBoard[i] = disk.BoardCharWithColor()
	}
	// Add possible moves to board
	for _, possibleMove := range moves {
		index := b.squareIndex(&possibleMove.Square)
		formattedBoard[index] = aurora.Yellow(strconv.Itoa(possibleMove.Value)).String()
		fmt.Printf("  %s\n", possibleMove)
	}
	// Print board with move positions
	fmt.Print("    ")
	for _, i := range b.indices {
		fmt.Printf(" %s", aurora.Bold(strconv.Itoa(i)))
	}
	for _, y := range b.indices {
		fmt.Printf("\n  %s", aurora.Bold(strconv.Itoa(y)))
		for _, x := range b.indices {
			fmt.Printf(" %s", formattedBoard[y*b.size+x])
		}
	}
	fmt.Println()
}

// PrintScore Print current score for both players.
func (b *Board) PrintScore() {
	black, white := b.playerScores()
	fmt.Printf("\n%s\n", b)
	fmt.Printf("Score: %d | %d\n", aurora.Magenta(black), aurora.Cyan(white))
}

// Result Returns the winning disk colour. Empty indicates a draw.
func (b *Board) Result() Disk {
	totalScore := b.score()
	switch {
	case totalScore > 0:
		return White
	case totalScore < 0:
		return Black
	default:
		return Empty
	}
}

// LogEntry Get board status string for game log.
func (b *Board) LogEntry() string {
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

// Check that the given square is valid (inside the board).
func (b *Board) checkSquare(square *Square) bool {
	return b.checkCoordinates(square.X, square.Y)
}

// Returns the state of the board (empty, white, black) at the given square.
// The boolean return value will be false if the square is outside the board.
func (b *Board) getSquare(square *Square) (Disk, bool) {
	if b.checkSquare(square) {
		index := b.squareIndex(square)
		return b.board[index], true
	}
	return Empty, false
}

// Map square to board index.
func (b *Board) squareIndex(square *Square) int {
	return square.Y*b.size + square.X
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

// Sets the given square to the given value.
func (b *Board) setSquare(square *Square, disk Disk) {
	if !b.checkSquare(square) {
		panic(fmt.Sprintf("Invalid coordinates: %s", square))
	}
	index := b.squareIndex(square)
	b.board[index] = disk
}

// Initialize game board with starting disk positions.
func initBoard(size int) []Disk {
	// Initialize game board with empty disks
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

// Initialize empty squares for the board.
func initEmptySquares(size int, board []Disk) mapset.Set[Square] {
	emptySquares := mapset.NewSet[Square]()
	for i, disk := range board {
		if disk == Empty {
			emptySquares.Add(Square{X: i % size, Y: i / size})
		}
	}
	return emptySquares
}

// String Format game board to string.
func (b *Board) String() string {
	// Horizontal indices
	numbers := make([]string, len(b.indices))
	for i, index := range b.indices {
		numbers[i] = strconv.Itoa(index)
	}
	text := fmt.Sprintf("  %s", aurora.Bold(strings.Join(numbers, " ")))
	for _, y := range b.indices {
		// Vertical index
		text += fmt.Sprintf("\n%s", aurora.Bold(strconv.Itoa(y)))
		// Row values
		for _, x := range b.indices {
			disk := b.board[y*b.size+x]
			text += fmt.Sprintf(" %s", disk.BoardCharWithColor())
		}
	}
	return text
}
