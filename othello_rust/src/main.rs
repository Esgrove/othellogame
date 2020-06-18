//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored;     // print with color to terminal

use colored::Colorize;

mod othello;
mod board;
mod player;
mod utils;

fn main() {
    println!("{}", "OTHELLO GAME: RUST".green());
    let mut game = othello::Othello::new();
    game.play();
}