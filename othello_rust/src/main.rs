//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

// print with color to terminal
extern crate colored;

#[macro_use]
extern crate log;

use clap::Parser;
use colored::Colorize;

use std::env;

mod board;
mod othello;
mod player;
mod utils;

/// Command line arguments
///
/// Basic info is read from `Cargo.toml`
/// See Clap `Derive` documentation for details:
/// <https://docs.rs/clap/latest/clap/_derive/index.html>
#[derive(Parser)]
#[command(
    author,
    version,
    about = "A simple Othello CLI game implementation.",
    long_about = "A simple Othello CLI game implementation.",
    arg_required_else_help = false
)]
struct Args {
    /// Optional Othello board size
    size: Option<usize>,
}

fn main() {
    // Uncomment to display backtrace in case of a panic
    env::set_var("RUST_BACKTRACE", "1");
    env_logger::init();

    println!("{}", "OTHELLO GAME - RUST".green().bold());

    // parse command line arguments using clap
    let args = Args::parse();
    // let args: Vec<String> = env::args().collect();

    let board_size: usize = {
        if let Some(size) = args.size {
            println!("Using board size: {size}");
            size
        } else {
            othello::Othello::get_board_size()
        }
    };
    let mut game = othello::Othello::new(board_size);
    game.play();
}
