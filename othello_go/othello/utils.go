//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2023
//==========================================================

package othello

import (
	"cmp"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"github.com/logrusorgru/aurora/v4"
	"runtime/debug"
)

const MinBoardSize int = 4
const MaxBoardSize int = 10
const DefaultBoardSize int = 8

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

// Move represents one possible disk placement for the given disk colour.
type Move struct {
	Square     Square
	Disk       Disk
	Value      int
	Directions []Step
}

// MovesDescending implements sort.Interface with custom sort order.
type MovesDescending []Move

// Settings Game settings.
type Settings struct {
	BoardSize    int
	AutoplayMode bool
	useDefaults   bool
	ShowHelpers  bool
	ShowLog      bool
	TestMode     bool
}

// PlayerSettings player settings.
type PlayerSettings struct {
	ShowHelpers bool
	TestMode    bool
}

func NewPlayerSettings(showHelpers, testMode bool) PlayerSettings {
	return PlayerSettings{
		ShowHelpers: showHelpers,
		TestMode:    testMode,
	}
}

func NewSettings(boardSize int, autoplayMode, useDefaults, showHelpers, showLog, testMode bool) Settings {
	return Settings{
		BoardSize:    boardSize,
		AutoplayMode: autoplayMode,
		useDefaults:   useDefaults,
		ShowHelpers:  showHelpers,
		ShowLog:      showLog,
		TestMode:     testMode,
	}
}

func DefaultSettings() Settings {
	return NewSettings(8, false, false, false, true, false)
}

func DefaultPlayerSettings() PlayerSettings {
	return NewPlayerSettings(true, false)
}
func (s Settings) ToPlayerSettings() PlayerSettings {
	return NewPlayerSettings(s.ShowHelpers, s.TestMode)
}

func (s Settings) String() string {
	return fmt.Sprintf(
		"Settings{BoardSize: %d, AutoplayMode: %t, useDefaults: %t, ShowHelpers: %t, ShowLog: %t, TestMode: %t}",
		s.BoardSize, s.AutoplayMode, s.useDefaults, s.ShowHelpers, s.ShowLog, s.TestMode,
	)
}

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

// BoardCharWithColor Returns a single character identifier string for the given disk.
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

// DiskString Returns the disk formatted as a coloured string.
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

// Opponent Returns the opposing disk colour for this disk.
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

// IsLessThan Custom comparison method since can't overload '<' operator in Go :(
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

func (m Move) ToLogEntry() string {
	return fmt.Sprintf("%s:%s,%d", m.Disk.BoardChar(), m.Square, m.Value)
}

// Calculate SHA256 hash for the given string.
func calculateSHA256(input string) string {
	data := []byte(input)
	hash := sha256.Sum256(data)
	hashString := hex.EncodeToString(hash[:])
	return hashString
}

// VersionInfo Returns formatted build version info string.
func VersionInfo() string {
	if info, ok := debug.ReadBuildInfo(); ok {
		goVersion := info.GoVersion
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
		return fmt.Sprintf("%s %s %s %s %s %s", VersionNumber, timestamp, GitBranch, commit, goVersion, arch)
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
