package othello_test

import (
	"othello_go/othello"
	"testing"
)

func TestBoardCanPlay(t *testing.T) {
	board := othello.NewBoard(8)
	if !board.CanPlay() {
		t.Error("new board should be playable")
	}
}

func TestBoardResult(t *testing.T) {
	board := othello.NewBoard(8)
	result := board.Result()
	if result != othello.Empty {
		t.Errorf("initial result = %v, want Empty", result)
	}

	moves := board.PossibleMoves(othello.Black)
	board.PlaceDisk(&moves[0])

	result = board.Result()
	if result != othello.Black {
		t.Errorf("after black move, result = %v, want Black", result)
	}
}

func TestBoardLogEntry(t *testing.T) {
	board8 := othello.NewBoard(8)
	expected8 := "___________________________WB______BW___________________________"
	if board8.LogEntry() != expected8 {
		t.Errorf("8x8 log = %q, want %q", board8.LogEntry(), expected8)
	}

	board4 := othello.NewBoard(4)
	expected4 := "_____WB__BW_____"
	if board4.LogEntry() != expected4 {
		t.Errorf("4x4 log = %q, want %q", board4.LogEntry(), expected4)
	}

	moves := board4.PossibleMoves(othello.Black)
	board4.PlaceDisk(&moves[0])
	expectedAfter := "____BBB__BW_____"
	if board4.LogEntry() != expectedAfter {
		t.Errorf("after move log = %q, want %q", board4.LogEntry(), expectedAfter)
	}
}
