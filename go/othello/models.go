//==========================================================
// Models
// Basic types and methods
// Akseli Lukkarila
// 2019-2026
//==========================================================

package othello

import (
	"fmt"
	"slices"

	"github.com/logrusorgru/aurora/v4"
)

// Disk Represents one game piece or lack of one.
type Disk int

const (
	Black Disk = -1
	Empty Disk = 0
	White Disk = 1
)

// Step Represents one step direction on the board.
type Step struct {
	X int
	Y int
}

// Square Represents one square location on the board.
type Square struct {
	X int
	Y int
}

// Direction Represents a continuous line of squares in one direction.
//
// The Step field determines the direction on the board,
// and Count describes how many consecutive squares in that direction there are.
type Direction struct {
	// Direction of travel on the board
	Step Step
	// Number of consecutive same colour squares along this direction
	Count int
}

// Move Represents one possible disk placement for the given disk colour.
type Move struct {
	Square     Square
	Disk       Disk
	Value      int
	Directions []Direction
}

// BoardChar Returns a single character identifier string for the given disk.
func (d Disk) BoardChar() string {
	switch d {
	case Black:
		return "B"
	case White:
		return "W"
	default:
		return "_"
	}
}

// BoardCharWithColor Returns a coloured single character identifier string for the given disk.
func (d Disk) BoardCharWithColor() string {
	return aurora.Colorize(d.BoardChar(), d.DiskColor()).String()
}

// DiskColor Return the associated colour for this disk.
func (d Disk) DiskColor() aurora.Color {
	switch d {
	case Black:
		return aurora.MagentaFg
	case White:
		return aurora.CyanFg
	default:
		return aurora.WhiteFg
	}
}

// DiskString Returns the disk formatted as a coloured string.
func (d Disk) DiskString() string {
	switch d {
	case Black:
		return aurora.Colorize("BLACK", d.DiskColor()).String()
	case White:
		return aurora.Colorize("WHITE", d.DiskColor()).String()
	default:
		return aurora.Colorize("EMPTY", d.DiskColor()).String()
	}
}

// Opponent Return the opposing disk colour for this disk.
func (d Disk) Opponent() Disk {
	switch d {
	case Black:
		return White
	case White:
		return Black
	default:
		return Empty
	}
}

// LogEntry Format move for log entry.
func (m Move) LogEntry() string {
	return fmt.Sprintf("%s:%s,%d", m.Disk.BoardChar(), m.Square, m.Value)
}

// AffectedSquares Get all the squares playing this move will change.
func (m Move) AffectedSquares() []Square {
	// Calculate the required size for the slice
	totalSize := 0
	for _, direction := range m.Directions {
		totalSize += direction.Count
	}
	paths := make([]Square, 0, totalSize)
	for _, direction := range m.Directions {
		pos := m.Square.Add(direction.Step)
		for range direction.Count {
			paths = append(paths, pos)
			pos = pos.Add(direction.Step)
		}
	}
	slices.SortFunc(paths, Square.Compare)
	return paths
}

// Add Return a new square from adding a step to a square.
func (s Square) Add(step Step) Square {
	return Square{X: s.X + step.X, Y: s.Y + step.Y}
}

// BoardIndex Get the index of this square on the board.
func (s Square) BoardIndex(boardSize int) int {
	return s.Y*boardSize + s.X
}

// Compare Compare two squares in ascending order by x-coordinate, then y-coordinate.
func (s Square) Compare(other Square) int {
	if s.X != other.X {
		return s.X - other.X
	}
	return s.Y - other.Y
}

// String Format step to string.
func (s Step) String() string {
	return fmt.Sprintf("[%d,%d]", s.X, s.Y)
}

// String Format square to string.
func (s Square) String() string {
	return fmt.Sprintf("(%d,%d)", s.X, s.Y)
}

// String Format move to string.
func (m Move) String() string {
	return fmt.Sprintf("Square: %s -> value: %d", m.Square, m.Value)
}
