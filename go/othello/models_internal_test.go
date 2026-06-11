package othello

import (
	"testing"
)

func TestSquareAddition(t *testing.T) {
	result := Square{X: 4, Y: 4}.Add(Step{X: -1, Y: 1})
	if result != (Square{X: 3, Y: 5}) {
		t.Errorf("got %v, want Square{3,5}", result)
	}

	result = result.Add(Step{X: -1, Y: -1})
	if result != (Square{X: 2, Y: 4}) {
		t.Errorf("got %v, want Square{2,4}", result)
	}

	result = result.Add(Step{X: -1, Y: -1})
	if result != (Square{X: 1, Y: 3}) {
		t.Errorf("got %v, want Square{1,3}", result)
	}

	result = result.Add(Step{X: -1, Y: -1})
	if result != (Square{X: 0, Y: 2}) {
		t.Errorf("got %v, want Square{0,2}", result)
	}

	result = result.Add(Step{X: -1, Y: -1})
	if result != (Square{X: -1, Y: 1}) {
		t.Errorf("got %v, want Square{-1,1}", result)
	}

	result = result.Add(Step{X: 1, Y: -1})
	if result != (Square{X: 0, Y: 0}) {
		t.Errorf("got %v, want Square{0,0}", result)
	}

	result = result.Add(Step{X: -1, Y: -1})
	if result != (Square{X: -1, Y: -1}) {
		t.Errorf("got %v, want Square{-1,-1}", result)
	}
}

func TestStepDirections(t *testing.T) {
	origin := Square{X: 1, Y: 1}
	tests := []struct {
		step     Step
		expected Square
	}{
		{Step{X: -1, Y: -1}, Square{X: 0, Y: 0}},
		{Step{X: -1, Y: 0}, Square{X: 0, Y: 1}},
		{Step{X: -1, Y: 1}, Square{X: 0, Y: 2}},
		{Step{X: 0, Y: -1}, Square{X: 1, Y: 0}},
		{Step{X: 0, Y: 1}, Square{X: 1, Y: 2}},
		{Step{X: 1, Y: -1}, Square{X: 2, Y: 0}},
		{Step{X: 1, Y: 0}, Square{X: 2, Y: 1}},
		{Step{X: 1, Y: 1}, Square{X: 2, Y: 2}},
	}
	for _, tc := range tests {
		result := origin.Add(tc.step)
		if result != tc.expected {
			t.Errorf("Square{1,1}.Add(%v) = %v, want %v", tc.step, result, tc.expected)
		}
	}
}

func TestDiskBoardChar(t *testing.T) {
	if Black.BoardChar() != "B" {
		t.Errorf("Black.BoardChar() = %q, want \"B\"", Black.BoardChar())
	}
	if Empty.BoardChar() != "_" {
		t.Errorf("Empty.BoardChar() = %q, want \"_\"", Empty.BoardChar())
	}
	if White.BoardChar() != "W" {
		t.Errorf("White.BoardChar() = %q, want \"W\"", White.BoardChar())
	}
}

func TestDiskOpponent(t *testing.T) {
	if Black.Opponent() != White {
		t.Errorf("Black.Opponent() = %v, want White", Black.Opponent())
	}
	if Empty.Opponent() != Empty {
		t.Errorf("Empty.Opponent() = %v, want Empty", Empty.Opponent())
	}
	if White.Opponent() != Black {
		t.Errorf("White.Opponent() = %v, want Black", White.Opponent())
	}
}

func TestMoveLogEntry(t *testing.T) {
	b := Move{
		Square:     Square{X: 3, Y: 2},
		Disk:       Black,
		Value:      10,
		Directions: []Direction{{Step: Step{X: 1, Y: 0}, Count: 10}},
	}
	if b.LogEntry() != "B:(3,2),10" {
		t.Errorf("got %q, want \"B:(3,2),10\"", b.LogEntry())
	}

	w := Move{
		Square:     Square{X: 0, Y: 0},
		Disk:       White,
		Value:      1,
		Directions: []Direction{{Step: Step{X: 1, Y: 0}, Count: 1}},
	}
	if w.LogEntry() != "W:(0,0),1" {
		t.Errorf("got %q, want \"W:(0,0),1\"", w.LogEntry())
	}
}
