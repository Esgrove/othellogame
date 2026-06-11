package othello

import (
	"testing"
)

func TestBoardInitialization(t *testing.T) {
	boardSize := 8
	board := NewBoard(boardSize)
	if board.size != boardSize {
		t.Errorf("board size = %d, want %d", board.size, boardSize)
	}
	// Check initial disk positions
	tests := []struct {
		square   Square
		expected Disk
	}{
		{Square{X: 3, Y: 3}, White},
		{Square{X: 4, Y: 4}, White},
		{Square{X: 3, Y: 4}, Black},
		{Square{X: 4, Y: 3}, Black},
		// Rest of the board should be empty
		{Square{X: 0, Y: 0}, Empty},
		{Square{X: 7, Y: 7}, Empty},
	}
	for _, tc := range tests {
		disk, ok := board.getSquare(&tc.square)
		if !ok {
			t.Fatalf("square %s should be inside the board", tc.square)
		}
		if disk != tc.expected {
			t.Errorf("square %s = %v, want %v", tc.square, disk, tc.expected)
		}
	}
}

func TestCanPlay(t *testing.T) {
	board := NewBoard(8)
	if !board.CanPlay() {
		t.Error("new board should be playable")
	}
}

func TestScoring(t *testing.T) {
	board := NewBoard(8)
	blackScore, whiteScore := board.playerScores()
	if blackScore != 2 {
		t.Errorf("black score = %d, want 2", blackScore)
	}
	if whiteScore != 2 {
		t.Errorf("white score = %d, want 2", whiteScore)
	}
	if board.score() != 0 {
		t.Errorf("total score = %d, want 0", board.score())
	}
}

func TestGameResult(t *testing.T) {
	board := NewBoard(8)
	result := board.Result()
	if result != Empty {
		t.Errorf("initial result = %v, want Empty", result)
	}

	moves := board.PossibleMoves(Black)
	board.PlaceDisk(&moves[0])

	result = board.Result()
	if result != Black {
		t.Errorf("after black move, result = %v, want Black", result)
	}
}

func TestLogEntry(t *testing.T) {
	board8 := NewBoard(8)
	expected8 := "___________________________WB______BW___________________________"
	if board8.LogEntry() != expected8 {
		t.Errorf("8x8 log = %q, want %q", board8.LogEntry(), expected8)
	}

	board4 := NewBoard(4)
	expected4 := "_____WB__BW_____"
	if board4.LogEntry() != expected4 {
		t.Errorf("4x4 log = %q, want %q", board4.LogEntry(), expected4)
	}

	moves := board4.PossibleMoves(Black)
	board4.PlaceDisk(&moves[0])
	expectedAfter := "____BBB__BW_____"
	if board4.LogEntry() != expectedAfter {
		t.Errorf("after move log = %q, want %q", board4.LogEntry(), expectedAfter)
	}
}
