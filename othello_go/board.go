//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2023
//==========================================================

package main

type Board struct {
	board      []Disk
	disk       Disk
	value      int
	directions []Step
}
