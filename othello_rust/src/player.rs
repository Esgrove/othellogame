//==========================================================
// Player
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2023
//==========================================================

extern crate colored;

use colored::Colorize;
use rand::{self, Rng};

use std::io::Write;
use std::{fmt, io};
use std::{thread, time::Duration};

use crate::board::Board;
use crate::colorprint::print_error;
use crate::utils::{Disk, Move, Square};

/// Defines one player that can be either human or computer controlled.
#[derive(Debug)]
pub(crate) struct Player {
    pub can_play: bool,
    disk: Disk,
    human: bool,
    rounds_played: u32,
    show_helpers: bool,
}

impl Player {
    /// Initialize new player for the given disk color.
    pub fn new(disk: Disk) -> Player {
        Player {
            disk,
            human: true,
            can_play: true,
            show_helpers: true,
            rounds_played: 0,
        }
    }

    /// Shorthand to initialize a new player for black disks.
    pub fn black() -> Player {
        Player::new(Disk::Black)
    }

    /// Shorthand to initialize a new player for white disks.
    pub fn white() -> Player {
        Player::new(Disk::White)
    }

    /// Play one round as this player.
    pub fn play_one_move(&mut self, board: &mut Board) {
        println!("Turn: {}", self.disk);
        let moves = board.possible_moves(self.disk);
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
            println!("{}", "  No moves available...".yellow());
        }
    }

    /// Reset player status for a new game.
    pub fn reset(&mut self) {
        self.can_play = true;
        self.rounds_played = 0;
    }

    /// Set the player as human or computer controlled.
    pub fn set_human(&mut self, is_human: bool) {
        self.human = is_human;
    }

    /// Return move chosen by computer.
    fn get_computer_move(&self, moves: &Vec<Move>) -> Move {
        println!("  Computer plays...");
        // Wait a bit and pick a random move
        thread::sleep(Duration::from_millis(
            rand::thread_rng().gen_range(1000..2000),
        ));
        let chosen_move = moves[rand::thread_rng().gen_range(0..moves.len())].clone();
        println!("  -> {}", chosen_move.square);
        chosen_move
    }

    /// Return move chosen by a human player.
    fn get_human_move(&self, moves: &[Move]) -> Move {
        loop {
            let square = Self::get_square();
            // Check that the chosen square is actually one of the possible moves
            if let Some(valid_move) = moves.iter().find(|m| m.square == square) {
                return valid_move.clone();
            }
            print_error(&format!(
                "  Can't place a {} disk in square {}!",
                self.disk, square
            ));
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
            print_error("  Give coordinates in the form 'x,y'!")
        }
    }
}

impl fmt::Display for Player {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(
            f,
            "{} | {} | Moves: {}",
            self.disk,
            if self.human { "Human   " } else { "Computer" },
            self.rounds_played
        )
    }
}
