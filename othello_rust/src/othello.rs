//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored;

use colored::*;

use crate::board::Board;
use crate::player::Player;
use crate::utils::Disk;
use std::cmp;
use std::io::{self, Write};

pub(crate) struct Othello {
    board: Board,
    player_black: Player,
    player_white: Player,
    rounds_played: u32,
    board_size: u8,
}

impl Othello {
    pub fn new(size: u8) -> Othello {
        Othello {
            board: Board::new(size),
            board_size: size,
            player_black: Player::new(Disk::BLACK),
            player_white: Player::new(Disk::WHITE),
            rounds_played: 0,
        }
    }

    pub fn play(&mut self) {
        loop {
            self.init_game();
            self.game_loop();
            self.print_result();
            if !Othello::get_answer("\nWould you like to play again", "y", "n") {
                break;
            }
        }
    }

    fn init_game(&mut self) {
        println!("Board size: {}", self.board_size);

        if Othello::get_answer("Would you like to play against the computer", "y", "n") {
            if Othello::get_answer("Would you like to play as black or white", "b", "w") {
                println!("{}", "black".cyan());
            } else {
                println!("{}", "white".magenta());
            }
        }

        println!(
            "{}\n{}\n{}",
            self.board, self.player_black, self.player_white
        );
        self.rounds_played = 0;
    }

    fn game_loop(&mut self) {
        while self.board.can_play()
            && (self.player_black.can_play() || self.player_white.can_play())
        {
            self.rounds_played += 1;
            println!(
                "{}",
                format!("\n=========== ROUND: {} ===========\n", self.rounds_played).bold()
            );
            self.player_black.play_one_move(&mut self.board);
            println!("--------------------------------");
            self.player_white.play_one_move(&mut self.board);
        }
    }

    fn print_result(&mut self) {
        println!("{}", "\n================================\n".bold());
        println!("{}", "The game is finished!\n".green());
        println!("Result:");
        self.print_status();
        println!();

        let winner = self.board.result();
        if winner == Disk::EMPTY {
            println!("The game ended in a tie...");
        } else {
            println!("The winner is {}!", winner);
        }
    }

    /// Print current board and player info.
    fn print_status(&mut self) {
        println!("{}", self.player_black);
        println!("{}\n", self.player_white);
        println!("{}", self.board);
    }

    // Associated (aka static) functions
    fn get_answer(text: &str, yes: &str, no: &str) -> bool {
        print!("{} ({}/{})? ", text, yes, no);
        let mut input = String::new();
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut input).expect("Input failed");
        return input.trim().to_lowercase() == *yes;
    }

    pub(crate) fn get_board_size() -> u8 {
        print!("Choose board size (default is 8): ");
        let mut input = String::new();
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut input).expect("Input failed");

        let board_size: i32 = input.trim().parse().unwrap();
        cmp::max(4, cmp::min(board_size, 8)) as u8
    }
}
