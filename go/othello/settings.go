package othello

import (
	"fmt"
)

// Game settings.
type Settings struct {
	BoardSize    int
	AutoplayMode bool
	CheckMode    bool
	ShowHelpers  bool
	ShowLog      bool
	TestMode     bool
	UseDefaults  bool
}

// Player settings.
type PlayerSettings struct {
	ShowHelpers bool
	CheckMode   bool
	TestMode    bool
}

func NewPlayerSettings(showHelpers, checkMode, testMode bool) PlayerSettings {
	return PlayerSettings{
		ShowHelpers: showHelpers,
		CheckMode:   checkMode,
		TestMode:    testMode,
	}
}

func NewSettings(
	boardSize int,
	autoplayMode, checkMode, showHelpers, showLog, testMode, useDefaults bool,
) Settings {
	return Settings{
		BoardSize:    boardSize,
		AutoplayMode: autoplayMode,
		CheckMode:    checkMode,
		ShowHelpers:  showHelpers,
		ShowLog:      showLog,
		TestMode:     testMode,
		UseDefaults:  useDefaults,
	}
}

func DefaultSettings() Settings {
	return NewSettings(8, false, false, false, false, true, false)
}

func DefaultPlayerSettings() PlayerSettings {
	return NewPlayerSettings(true, false, false)
}
func (s Settings) ToPlayerSettings() PlayerSettings {
	return NewPlayerSettings(s.ShowHelpers, s.CheckMode, s.TestMode)
}

func (s Settings) String() string {
	return fmt.Sprintf(
		`Settings:
			BoardSize: %d
			AutoplayMode: %t
			CheckMode: %t
			ShowHelpers: %t
			ShowLog: %t
			TestMode: %t
			useDefaults: %t
		`,
		s.BoardSize, s.AutoplayMode, s.CheckMode, s.ShowHelpers, s.ShowLog, s.TestMode, s.UseDefaults,
	)
}
