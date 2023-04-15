//==========================================================
// Player
// Play Othello (Reversi) on the command line
// Akseli Lukkarila
//==========================================================

extern crate colored;

use colored::Colorize;
use rand::{self, Rng};

use std::io::Write;
use std::{fmt, io};
use std::{thread, time::Duration};

use crate::board::Board;
use crate::utils::{Disk, Move, Square};

#[derive(Debug)]
pub(crate) struct Player {
    color: Disk,
    human: bool,
    can_play: bool,
    show_helpers: bool,
    rounds_played: u32,
}

impl Player {
    pub fn new(color: Disk) -> Player {
        Player {
            color,
            human: true,
            can_play: true,
            show_helpers: true,
            rounds_played: 0,
        }
    }

    pub fn black() -> Player {
        Player {
            color: Disk::Black,
            human: true,
            can_play: true,
            show_helpers: true,
            rounds_played: 0,
        }
    }

    pub fn white() -> Player {
        Player {
            color: Disk::White,
            human: true,
            can_play: true,
            show_helpers: true,
            rounds_played: 0,
        }
    }

    /// Play one round as this player.
    pub fn play_one_move(&mut self, board: &mut Board) {
        println!("Turn: {}", self.color);
        let moves = board.possible_moves(self.color);
        if !moves.is_empty() {
            self.can_play = true;
            if self.human && self.show_helpers {
                board.print_moves(&moves)
            }
            let chosen_move = if self.human {
                self.get_human_move(&moves)
            } else {
                self.get_computer_move(&moves)
            };
            board.place_disk(&chosen_move);
            board.print_score();
            self.rounds_played += 1;
            thread::sleep(Duration::from_secs(1));
        } else {
            self.can_play = false;
            println!("{}", "  No moves available...\n".yellow());
        }
    }

    /// Return move chosen by computer.
    fn get_computer_move(&self, moves: &Vec<Move>) -> Move {
        println!("  Computer plays...");
        // wait a bit and pick a random move
        thread::sleep(Duration::from_millis(rand::thread_rng().gen_range(1000..2000)));
        let chosen_move = moves[rand::thread_rng().gen_range(0..moves.len())].clone();
        println!("  -> {}", chosen_move.square);
        chosen_move
    }

    /// Return move chosen by a human player.
    fn get_human_move(&self, moves: &[Move]) -> Move {
        loop {
            let square = Self::get_square();
            // check if given square is one of the possible moves
            if let Some(valid_move) = moves.iter().find(|m| m.square == square) {
                return valid_move.clone();
            }
            println!("  Can't place a {} disk in square {}!\n", self.color, square);
        }
    }

    /// Ask human player for square coordinates.
    fn get_square() -> Square {
        loop {
            print!("  Give disk position (x,y): ");
            let mut input = String::new();
            io::stdout().flush().unwrap();
            io::stdin().read_line(&mut input).expect("Input failed");
            let values: Vec<&str> = input.trim().split(',').collect();
            // TODO: nicer error handling
            if values.len() == 2 {
                let x: isize = values[0].parse().unwrap_or(-1);
                let y: isize = values[1].parse().unwrap_or(-1);
                if x >= 0 && y >= 0 {
                    return Square { x, y };
                }
            }
            println!("{}", "  Give coordinates in the form 'x,y'\n".red())
        }
    }

    pub fn set_human(&mut self, is_human: bool) {
        self.human = is_human;
    }

    /// Reset player status for a new game.
    pub fn reset(&mut self) {
        self.can_play = true;
        self.rounds_played = 0;
    }

    pub fn can_play(&self) -> bool {
        self.can_play
    }
}

impl fmt::Display for Player {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(
            f,
            "{} | {} | {}",
            self.color,
            if self.human { "Human   " } else { "Computer" },
            self.rounds_played
        )
    }
}
