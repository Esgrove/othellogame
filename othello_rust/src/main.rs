//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

// print with color to terminal
extern crate colored;

use std::env;

use colored::Colorize;

mod board;
mod othello;
mod player;
mod utils;

fn main() {
    println!("{}", "OTHELLO GAME - RUST".green());
    let args: Vec<String> = env::args().collect();
    let mut board_size: u8 = 0;
    if args.len() == 2 {
        let cmd = &args[1];
        board_size = cmd.parse().unwrap_or(0);
    }
    if board_size <= 0 {
        board_size = othello::Othello::get_board_size();
    }
    let mut game = othello::Othello::new(board_size);
    game.play();
}
