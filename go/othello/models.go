package othello

import (
	"fmt"
	"sort"

	"github.com/logrusorgru/aurora/v4"
)

// Disk enum
type Disk int

const (
	Black Disk = -1
	Empty Disk = 0
	White Disk = 1
)

// Represents a step direction on the board.
type Step struct {
	X int
	Y int
}

// Represents one square location on the board.
type Square struct {
	X int
	Y int
}

// Represents one possible disk placement for the given disk colour.
type Move struct {
	Square     Square
	Disk       Disk
	Value      int
	Directions []StepCount
}

// Implements sort.Interface for a slice of Square objects
type Squares []Square

// Hold Step and number of steps since Go lacks a tuple / pair construct
type StepCount struct {
	Step  Step
	Count int
}

// Implements sort.Interface with custom sort order.
type MovesDescending []Move

func (m MovesDescending) Len() int      { return len(m) }
func (m MovesDescending) Swap(i, j int) { m[i], m[j] = m[j], m[i] }
func (m MovesDescending) Less(i, j int) bool {
	if m[i].Value > m[j].Value {
		return true
	} else if m[i].Value == m[j].Value {
		return m[i].Square.IsLessThan(m[j].Square)
	}
	return false
}

func (s Squares) Len() int           { return len(s) }
func (s Squares) Less(i, j int) bool { return s[i].IsLessThan(s[j]) }
func (s Squares) Swap(i, j int)      { s[i], s[j] = s[j], s[i] }

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

// Returns a single character identifier string for the given disk.
func (d Disk) BoardCharWithColor() string {
	switch d {
	case Black:
		return aurora.Magenta("B").String()
	case White:
		return aurora.Cyan("W").String()
	default:
		return aurora.White("_").String()
	}
}

// Returns the disk formatted as a coloured string.
func (d Disk) DiskString() string {
	switch d {
	case Black:
		return aurora.Magenta("BLACK").String()
	case White:
		return aurora.Cyan("WHITE").String()
	default:
		return aurora.White("EMPTY").String()
	}
}

// Returns the opposing disk colour for this disk.
func (d Disk) Opponent() Disk {
	if d == Black {
		return White
	} else if d == White {
		return Black
	}
	return Empty
}

func (s Step) String() string {
	return fmt.Sprintf("[%d,%d]", s.X, s.Y)
}

func (s Square) Add(step Step) Square {
	return Square{X: s.X + step.X, Y: s.Y + step.Y}
}

// Custom comparison method since can't overload '<' operator in Go :(
func (s Square) IsLessThan(other Square) bool {
	if s.X < other.X {
		return true
	} else if s.X == other.X {
		return s.Y < other.Y
	}
	return false
}

func (s Square) String() string {
	return fmt.Sprintf("(%d,%d)", s.X, s.Y)
}

func (m Move) String() string {
	return fmt.Sprintf("Square: %s -> value: %d", m.Square, m.Value)
}

func (m Move) LogEntry() string {
	return fmt.Sprintf("%s:%s,%d", m.Disk.BoardChar(), m.Square, m.Value)
}

// Get all the squares playing this move will change
func (m Move) AffectedSquares() []Square {
	var paths Squares
	for _, direction := range m.Directions {
		pos := m.Square.Add(direction.Step)
		for i := 0; i < int(direction.Count); i++ {
			paths = append(paths, pos)
			pos = pos.Add(direction.Step)
		}
	}
	sort.Sort(paths)
	return paths
}
