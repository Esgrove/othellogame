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

#[macro_use]
extern crate log;

mod board;
mod othello;
mod player;
mod utils;

fn main() {
    // Uncomment to display backtrace in case of a panic
    env::set_var("RUST_BACKTRACE", "1");
    env_logger::init();

    println!("{}", "OTHELLO GAME - RUST".green());
    let args: Vec<String> = env::args().collect();
    let board_size: usize = {
        if args.len() >= 2 {
            match args[1].parse() {
                Ok(n) => n,
                Err(_) => othello::Othello::get_board_size(),
            }
        } else {
            othello::Othello::get_board_size()
        }
    };
    let mut game = othello::Othello::new(board_size);
    game.play();
}
