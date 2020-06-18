//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored;  // print with color to terminal

use colored::*;

use std::cmp;
use std::io::{self, Write};
use crate::board::Board;
use crate::player::Player;
use crate::utils::Disk;

pub struct Othello {
    board: Board,
    player_black: Player,
    player_white: Player,
    rounds_played: i32
}

impl Othello {
    pub fn new() -> Othello {
        Othello {
            board: Board::default(),
            player_black: Player::new(Disk::BLACK),
            player_white: Player::new(Disk::WHITE),
            rounds_played: 0
        }
    }

    pub fn play(&mut self) {
        loop {
            self.init_game();

            if !Othello::get_answer("\nWould you like to play again", "y", "n") {
                break;
            }
        }
    }

    fn init_game(&mut self) {
        let board_size: u8 = Othello::get_board_size();
        println!("Board size: {}", board_size);

        if Othello::get_answer("Would you like to play against the computer", "y", "n") {
            if Othello::get_answer("Would you like to play as black or white", "b", "w") {
                println!("{}", "black".cyan());
            } else {
                println!("{}", "white".magenta());
            }
        }

        println!(
            "{}\n{}\n{}",
            self.board.bold(),
            self.player_black.green(),
            self.player_white.red()
        );
        self.rounds_played = 0;
    }

    // Associated (aka static) functions
    fn get_answer(text: &str, yes: &str, no: &str) -> bool {
        print!("{} ({}/{})? ", text, yes, no);
        let mut input = String::new();
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut input).expect("Input failed");
        return input.trim().to_lowercase() == yes.to_string();
    }

    fn get_board_size() -> u8 {
        print!("Choose board size (default is 8): ");
        let mut input = String::new();
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut input).expect("Input failed");

        let board_size: i32 = input.trim().parse().unwrap();
        return cmp::max(4, cmp::min(board_size, 8)) as u8;
    }
}
