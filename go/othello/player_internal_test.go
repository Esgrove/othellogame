package othello

import (
	"testing"
)

func TestNewPlayer(t *testing.T) {
	player := NewPlayer(Black, DefaultPlayerSettings())
	if player.disk != Black {
		t.Errorf("new player disk = %v, want Black", player.disk)
	}
	if !player.Human() {
		t.Error("new player should be human")
	}
	if !player.CanPlay {
		t.Error("new player should be able to play")
	}
	if player.roundsPlayed != 0 {
		t.Errorf("new player roundsPlayed = %d, want 0", player.roundsPlayed)
	}
	if player.settings != DefaultPlayerSettings() {
		t.Errorf("new player settings = %v, want %v", player.settings, DefaultPlayerSettings())
	}
}

func TestResetPlayer(t *testing.T) {
	player := NewPlayer(White, DefaultPlayerSettings())
	player.CanPlay = false
	player.roundsPlayed = 10
	player.Reset()
	if !player.CanPlay {
		t.Error("reset player should be able to play")
	}
	if player.roundsPlayed != 0 {
		t.Errorf("reset player roundsPlayed = %d, want 0", player.roundsPlayed)
	}
}

func TestSetHuman(t *testing.T) {
	player := BlackPlayer(DefaultPlayerSettings())
	player.SetComputer()
	if !player.IsComputer() {
		t.Error("player should be computer")
	}
	if player.playerType != Computer {
		t.Errorf("player type = %v, want Computer", player.playerType)
	}

	player.SetHuman()
	if !player.Human() {
		t.Error("player should be human")
	}
	if player.playerType != Human {
		t.Errorf("player type = %v, want Human", player.playerType)
	}
}

func TestPlayerTypeString(t *testing.T) {
	player := BlackPlayer(DefaultPlayerSettings())
	if player.typeString() != "Human   " {
		t.Errorf("got %q, want \"Human   \"", player.typeString())
	}

	player.SetComputer()
	if player.typeString() != "Computer" {
		t.Errorf("got %q, want \"Computer\"", player.typeString())
	}
}
