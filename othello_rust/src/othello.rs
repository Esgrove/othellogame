//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored;
use crate::colored::Colorize;

use log::debug;

use std::io::{self, Write};

use crate::board::Board;
use crate::player::Player;
use crate::utils::Disk;

pub(crate) struct Othello {
    board: Board,
    player_black: Player,
    player_white: Player,
    rounds_played: u32,
    board_size: usize,
}

impl Othello {
    pub fn new(size: usize) -> Othello {
        Othello {
            board: Board::new(size),
            board_size: size,
            player_black: Player::black(),
            player_white: Player::white(),
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
        self.rounds_played = 0;

        if Othello::get_answer("Would you like to play against the computer", "y", "n") {
            if Othello::get_answer("Would you like to play as black or white", "b", "w") {
                self.player_white.set_human(false);
            } else {
                self.player_black.set_human(false);
            }
        }
        println!("{}", "\nPlayers:".bold());
        self.print_status();
    }

    fn game_loop(&mut self) {
        debug!("Running game loop");
        while self.board.can_play() && (self.player_black.can_play() || self.player_white.can_play()) {
            self.rounds_played += 1;
            println!(
                "{}",
                format!("\n=========== ROUND: {} ===========", self.rounds_played).bold()
            );
            self.player_black.play_one_move(&mut self.board);
            println!("--------------------------------");
            self.player_white.play_one_move(&mut self.board);
        }
    }

    fn print_result(&self) {
        println!("{}", "\n================================".bold());
        println!("{}", "The game is finished!\n".green());
        println!("Result:");
        self.print_status();
        println!();

        let winner = self.board.result();
        if winner == Disk::Empty {
            println!("The game ended in a tie...");
        } else {
            println!("The winner is {}!", winner);
        }
    }

    /// Print current board and player info.
    fn print_status(&self) {
        println!("{}", self.player_black);
        println!("{}\n", self.player_white);
        println!("{}", self.board);
    }

    // Associated aka static functions (no self parameter)
    fn get_answer(text: &str, yes: &str, no: &str) -> bool {
        print!("{} ({}/{})? ", text, yes, no);
        let mut input = String::new();
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut input).expect("Input failed");
        return input.trim().to_lowercase() == *yes;
    }

    pub(crate) fn get_board_size() -> usize {
        print!("Choose board size (default is 8): ");
        let mut input = String::new();
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut input).expect("Input failed");
        let board_size: usize = input.trim().parse().unwrap_or(8);
        board_size.clamp(4, 16)
    }
}
