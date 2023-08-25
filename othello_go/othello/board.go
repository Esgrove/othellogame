//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2023
//==========================================================

package othello

import (
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

// Initialize game board with empty disks and starting positions.
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
	board[row*size+row] = Black
	board[row*size+col] = White
	board[col*size+row] = White
	board[col*size+col] = Black
	return board
}

func (d Disk) Opponent() Disk {
	if d == Black {
		return White
	} else if d == White {
		return Black
	}
	return Empty
}

func (d Disk) BoardChar() string {
	switch d {
	case Black:
		return "B"
	case White:
		return "W"
	default:
		return "."
	}
}

func (b *Board) CanPlay() bool {
	return b.emptySquares.Cardinality() > 0
}

func (b *Board) CheckCoordinates(x, y int) bool {
	return x >= 0 && x < b.size && y >= 0 && y < b.size
}

func (b *Board) getSquare(square *Square) Disk {
	if b.CheckCoordinates(square.X, square.Y) {
		return b.board[square.Y*b.size+square.X]
	}
	return Empty
}

func (b *Board) setSquare(square *Square, disk Disk) {
	if !b.CheckCoordinates(square.X, square.Y) {
		panic("Invalid coordinates")
	}
	b.board[square.Y*b.size+square.X] = disk
}

func (b *Board) PlaceDisk(playerMove *Move) {
	start := playerMove.Square
	if b.getSquare(&start) != Empty {
		panic(fmt.Sprintf("Trying to place disk to an occupied square: %v!", start))
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

func (b *Board) PossibleMoves(disk Disk) []Move {
	var moves []Move
	opposingDisk := disk.Opponent()
	for square := range b.emptySquares.Iter() {
		var value int
		var directions []Step
		for _, step := range b.directions {
			pos := square.Add(step)
			if b.getSquare(&pos) != opposingDisk {
				continue
			}
			var numSteps int
			for b.getSquare(&pos) == opposingDisk {
				numSteps++
				pos = pos.Add(step)
			}
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
		sort.SliceStable(moves, func(i, j int) bool {
			return moves[i].Value > moves[j].Value
		})
	}
	return moves
}

func (b *Board) PlayerScores() (int, int) {
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

func (b *Board) PrintScore() {
	black, white := b.PlayerScores()
	fmt.Println(b)
	fmt.Printf("Score: %s | %s\n", aurora.Magenta(black), aurora.Cyan(white))
}

func (b *Board) PrintMoves(moves []Move) {
	fmt.Printf("  Possible plays (%d):\n", len(moves))
	formattedBoard := make([]string, len(b.board))
	for i, disk := range b.board {
		formattedBoard[i] = disk.BoardChar()
	}
	for _, possibleMove := range moves {
		index := possibleMove.Square.Y*b.size + possibleMove.Square.X
		formattedBoard[index] = fmt.Sprintf("%d", possibleMove.Value)
		fmt.Printf("  %v\n", possibleMove)
	}
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

func (b *Board) Result() Disk {
	sum := b.Score()
	switch {
	case sum > 0:
		return White
	case sum < 0:
		return Black
	default:
		return Empty
	}
}

func (b *Board) Score() int {
	var sum int
	for _, disk := range b.board {
		sum += int(disk)
	}
	return sum
}
