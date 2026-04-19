package othello_test

import (
	"othello_go/othello"
	"testing"
)

func TestClamp(t *testing.T) {
	tests := []struct {
		value, min, max, expected int
	}{
		{0, 0, 0, 0},
		{0, 0, 1, 0},
		{1, 0, 1, 1},
		{2, 0, 1, 1},
		{-2, -1, 0, -1},
		{0, -1, 1, 0},
	}
	for _, tc := range tests {
		result := othello.Clamp(tc.value, tc.min, tc.max)
		if result != tc.expected {
			t.Errorf("Clamp(%d, %d, %d) = %d, want %d", tc.value, tc.min, tc.max, result, tc.expected)
		}
	}
}

func TestCalculateSHA256Simple(t *testing.T) {
	input := "test"
	expected := "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
	result := othello.CalculateSHA256(input)
	if result != expected {
		t.Errorf("CalculateSHA256(%q) = %q, want %q", input, result, expected)
	}
}

func TestCalculateSHA256LogLine(t *testing.T) {
	input := "01: B:(0,1),1;____BBB__BW_____"
	expected := "d087cd890b81b7153cc870fba7372396e91cdcd4b6d13b82bbc88b5a4eb889d1"
	result := othello.CalculateSHA256(input)
	if result != expected {
		t.Errorf("got %q, want %q", result, expected)
	}
}

func TestCalculateSHA256FullLog(t *testing.T) {
	input := "01: B:(0,1),1;____BBB__BW_____\n" +
		"02: W:(0,0),1;W___BWB__BW_____\n" +
		"03: B:(1,0),1;WB__BBB__BW_____\n" +
		"04: W:(0,2),2;WB__WBB_WWW_____\n" +
		"05: B:(0,3),1;WB__WBB_WBW_B___\n" +
		"06: W:(2,0),3;WWW_WWW_WBW_B___\n" +
		"07: B:(3,0),1;WWWBWWB_WBW_B___\n" +
		"08: W:(1,3),1;WWWBWWB_WWW_BW__\n" +
		"09: B:(2,3),2;WWWBWWB_WWB_BBB_\n" +
		"10: W:(3,2),2;WWWBWWW_WWWWBBB_\n" +
		"11: B:(3,1),1;WWWBWWWBWWBWBBB_\n" +
		"12: W:(3,3),1;WWWBWWWBWWWWBBBW"
	expected := "ad4679949110ed7997aa1bf09441a7eb491b498189c03685ffdd6ddbb70e3c22"
	result := othello.CalculateSHA256(input)
	if result != expected {
		t.Errorf("got %q, want %q", result, expected)
	}
}
