package othello_test

import (
	"othello_go/othello"
	"testing"
)

func TestStepDirections(t *testing.T) {
	origin := othello.Square{X: 1, Y: 1}
	tests := []struct {
		step     othello.Step
		expected othello.Square
	}{
		{othello.Step{X: -1, Y: -1}, othello.Square{X: 0, Y: 0}},
		{othello.Step{X: -1, Y: 0}, othello.Square{X: 0, Y: 1}},
		{othello.Step{X: -1, Y: 1}, othello.Square{X: 0, Y: 2}},
		{othello.Step{X: 0, Y: -1}, othello.Square{X: 1, Y: 0}},
		{othello.Step{X: 0, Y: 1}, othello.Square{X: 1, Y: 2}},
		{othello.Step{X: 1, Y: -1}, othello.Square{X: 2, Y: 0}},
		{othello.Step{X: 1, Y: 0}, othello.Square{X: 2, Y: 1}},
		{othello.Step{X: 1, Y: 1}, othello.Square{X: 2, Y: 2}},
	}
	for _, tc := range tests {
		result := origin.Add(tc.step)
		if result != tc.expected {
			t.Errorf("Square{1,1}.Add(%v) = %v, want %v", tc.step, result, tc.expected)
		}
	}
}

func TestDiskBoardChar(t *testing.T) {
	if othello.Black.BoardChar() != "B" {
		t.Errorf("Black.BoardChar() = %q, want \"B\"", othello.Black.BoardChar())
	}
	if othello.Empty.BoardChar() != "_" {
		t.Errorf("Empty.BoardChar() = %q, want \"_\"", othello.Empty.BoardChar())
	}
	if othello.White.BoardChar() != "W" {
		t.Errorf("White.BoardChar() = %q, want \"W\"", othello.White.BoardChar())
	}
}

func TestDiskOpponent(t *testing.T) {
	if othello.Black.Opponent() != othello.White {
		t.Errorf("Black.Opponent() = %v, want White", othello.Black.Opponent())
	}
	if othello.Empty.Opponent() != othello.Empty {
		t.Errorf("Empty.Opponent() = %v, want Empty", othello.Empty.Opponent())
	}
	if othello.White.Opponent() != othello.Black {
		t.Errorf("White.Opponent() = %v, want Black", othello.White.Opponent())
	}
}

func TestMoveLogEntry(t *testing.T) {
	b := othello.Move{
		Square:     othello.Square{X: 3, Y: 2},
		Disk:       othello.Black,
		Value:      10,
		Directions: []othello.Direction{{Step: othello.Step{X: 1, Y: 0}, Count: 10}},
	}
	if b.LogEntry() != "B:(3,2),10" {
		t.Errorf("got %q, want \"B:(3,2),10\"", b.LogEntry())
	}

	w := othello.Move{
		Square:     othello.Square{X: 0, Y: 0},
		Disk:       othello.White,
		Value:      1,
		Directions: []othello.Direction{{Step: othello.Step{X: 1, Y: 0}, Count: 1}},
	}
	if w.LogEntry() != "W:(0,0),1" {
		t.Errorf("got %q, want \"W:(0,0),1\"", w.LogEntry())
	}
}
