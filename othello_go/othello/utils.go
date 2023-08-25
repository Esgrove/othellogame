//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2023
//==========================================================

package othello

import (
	"cmp"
	"fmt"
	"runtime/debug"
)

// Disk enum
type Disk int

const (
	Black Disk = -1
	Empty Disk = 0
	White Disk = 1
)

// Step represents a step direction on the board.
type Step struct {
	X int
	Y int
}

// Square represents one square location on the board.
type Square struct {
	X int
	Y int
}

// Move represents one possible disk placement for given disk colour.
type Move struct {
	Square     Square
	Disk       Disk
	Value      int
	Directions []Step
}

func (s Square) Add(step Step) Square {
	return Square{X: s.X + step.X, Y: s.Y + step.Y}
}

func (s Step) String() string {
	return fmt.Sprintf("(%d, %d)", s.X, s.Y)
}

func (m Move) String() string {
	return fmt.Sprintf("Move{Square: %v, Disk: %v, Value: %d, Directions: %v}",
		m.Square, m.Disk, m.Value, m.Directions)
}

// VersionInfo Returns build version info string.
func VersionInfo() string {
	if info, ok := debug.ReadBuildInfo(); ok {
		version := info.GoVersion
		commit := ""
		timestamp := ""
		arch := ""
		for _, setting := range info.Settings {
			if setting.Key == "vcs.revision" {
				commit = setting.Value
			}
			if setting.Key == "vcs.time" {
				timestamp = setting.Value
			}
			if setting.Key == "GOARCH" {
				arch = setting.Value
			}
		}
		return fmt.Sprintf("Othello Go %s %s %s %s", timestamp, commit, version, arch)
	}
	return ""
}

// Clamp a value to the given range.
func Clamp[T cmp.Ordered](value T, minimum T, maximum T) T {
	if minimum > maximum {
		panic("Minimum value should be less than or equal to maximum!")
	}
	return max(minimum, min(value, maximum))
}
