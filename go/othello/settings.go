//==========================================================
// Settings
// Defines game settings
// Akseli Lukkarila
// 2019-2026
//==========================================================

package othello

import (
	"fmt"
)

const MinBoardSize int = 4
const MaxBoardSize int = 10
const DefaultBoardSize int = 8

// Settings Game settings.
type Settings struct {
	BoardSize    int
	AutoplayMode bool
	CheckMode    bool
	ShowHelpers  bool
	ShowLog      bool
	TestMode     bool
	UseDefaults  bool
}

// PlayerSettings Player settings.
type PlayerSettings struct {
	ShowHelpers bool
	CheckMode   bool
	TestMode    bool
}

// ToPlayerSettings Get player setting values from overall game settings.
func (s Settings) ToPlayerSettings() PlayerSettings {
	return PlayerSettings{
		ShowHelpers: s.ShowHelpers,
		CheckMode:   s.CheckMode,
		TestMode:    s.TestMode,
	}
}

// DefaultSettings Get the default game settings.
func DefaultSettings() Settings {
	return Settings{
		BoardSize:    8,
		AutoplayMode: false,
		CheckMode:    false,
		ShowHelpers:  true,
		ShowLog:      false,
		TestMode:     false,
		UseDefaults:  false,
	}
}

// DefaultPlayerSettings Get the default player settings.
func DefaultPlayerSettings() PlayerSettings {
	return PlayerSettings{
		ShowHelpers: true,
		CheckMode:   false,
		TestMode:    false,
	}
}

// String Format settings to string.
func (s Settings) String() string {
	return fmt.Sprintf(
		"Settings:\n"+
			"  board_size: %d\n"+
			"  autoplay: %t\n"+
			"  check_mode: %t\n"+
			"  use_defaults: %t\n"+
			"  show_helpers: %t\n"+
			"  show_log: %t\n"+
			"  test_mode: %t",
		s.BoardSize,
		s.AutoplayMode,
		s.CheckMode,
		s.UseDefaults,
		s.ShowHelpers,
		s.ShowLog,
		s.TestMode,
	)
}
