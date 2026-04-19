package othello_test

import (
	"othello_go/othello"
	"testing"
)

func TestNewPlayer(t *testing.T) {
	player := othello.BlackPlayer(othello.NewPlayerSettings(true, false, true))
	if !player.CanPlay {
		t.Error("new player should be able to play")
	}
	if !player.Human() {
		t.Error("new player should be human")
	}
	if player.IsComputer() {
		t.Error("new player should not be computer")
	}
}

func TestResetPlayer(t *testing.T) {
	player := othello.WhitePlayer(othello.NewPlayerSettings(true, false, true))
	player.CanPlay = false
	player.Reset()
	if !player.CanPlay {
		t.Error("reset player should be able to play")
	}
}

func TestSetHumanAndComputer(t *testing.T) {
	player := othello.BlackPlayer(othello.NewPlayerSettings(true, false, true))
	if !player.Human() {
		t.Error("player should be human")
	}

	player.SetComputer()
	if !player.IsComputer() {
		t.Error("player should be computer")
	}
	if player.Human() {
		t.Error("player should not be human")
	}

	player.SetHuman()
	if !player.Human() {
		t.Error("player should be human")
	}
	if player.IsComputer() {
		t.Error("player should not be computer")
	}
}
