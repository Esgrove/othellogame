//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2023
//==========================================================

package main

type Player struct {
	canPlay      bool
	color        Disk
	human        bool
	roundsPlayed int
	showHelpers  bool
}
