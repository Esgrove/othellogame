//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored; // print with color to terminal
extern crate ferris_says; // print Rust mascot "Ferris"

use colored::*;
use ferris_says::say;

use std::cmp;
use std::io::{self, Write};

//==========================================================

fn main() {
    let stdout = io::stdout();
    let mut writer = io::BufWriter::new(stdout.lock());
    say(b"      OTHELLO GAME IN RUST", 32, &mut writer).unwrap();
    Write::flush(&mut writer).unwrap();

    print!("Choose board size (4, 6, 8, default is 8): ");
    io::stdout().flush().unwrap();
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Input failed");

    let mut board_size: u32 = input.trim().parse().unwrap();
    board_size = cmp::max(4, cmp::min(board_size, 8));
    println!("Board size: {}", board_size);

    print!("Would you like to play against the computer (y/n)? ");
    io::stdout().flush().unwrap();
    input = String::new();
    io::stdin().read_line(&mut input).expect("Input failed");
    if input.trim() == "y" {
        print!("Would you like to play as black or white (b/w)? ");
        io::stdout().flush().unwrap();
        input = String::new();
        io::stdin().read_line(&mut input).expect("Input failed");
        if input.trim() == "b" {
            println!("{}", "black".cyan());
        } else {
            println!("{}", "white".magenta());
        }
    }

    println!(
        "{}\n{}\n{}\n",
        "Players:".bold(),
        "temp1".green(),
        "temp2".red()
    );
}
