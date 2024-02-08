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
pub struct Player {
    pub can_play: bool,
    disk: Disk,
    human: bool,
    rounds_played: u32,
    settings: PlayerSettings,
}

/// Player settings.
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct PlayerSettings {
    pub show_helpers: bool,
    pub test_mode: bool,
}

impl Default for PlayerSettings {
    fn default() -> Self {
        PlayerSettings {
            show_helpers: true,
            test_mode: false,
        }
    }
}

impl Player {
    /// Initialize new player for the given disk color.
    pub fn new(disk: Disk, settings: PlayerSettings) -> Player {
        Player {
            disk,
            human: true,
            can_play: true,
            rounds_played: 0,
            settings,
        }
    }

    /// Shorthand to initialize a new player for black disks.
    pub fn black(settings: PlayerSettings) -> Player {
        Player::new(Disk::Black, settings)
    }

    /// Shorthand to initialize a new player for white disks.
    pub fn white(settings: PlayerSettings) -> Player {
        Player::new(Disk::White, settings)
    }

    /// Play one round as this player.
    pub fn play_one_move(&mut self, board: &mut Board, disable_prints: &bool) -> Option<String> {
        if !disable_prints {
            println!("Turn: {}", self.disk);
        }
        let moves = board.possible_moves(self.disk);
        if !moves.is_empty() {
            self.can_play = true;
            if self.human && self.settings.show_helpers {
                board.print_possible_moves(&moves)
            }
            let chosen_move = if self.human {
                self.get_human_move(&moves)
            } else {
                self.get_computer_move(&moves, disable_prints)
            };
            board.place_disk(&chosen_move);
            if !disable_prints {
                board.print_score();
            }
            self.rounds_played += 1;
            if !self.settings.test_mode {
                thread::sleep(Duration::from_secs(1));
            }
            Some(chosen_move.to_log_entry())
        } else {
            self.can_play = false;
            if !disable_prints {
                println!("{}", "  No moves available...".yellow());
            }
            None
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
    fn get_computer_move(&self, moves: &[Move], disable_prints: &bool) -> Move {
        if !disable_prints {
            println!("  Computer plays...");
        }
        let chosen_move = if self.settings.test_mode {
            moves[0].clone()
        } else {
            // Wait a bit and pick a random move
            thread::sleep(Duration::from_millis(
                rand::thread_rng().gen_range(1000..2000),
            ));
            moves[rand::thread_rng().gen_range(0..moves.len())].clone()
        };
        if !disable_prints {
            println!("  {} -> {}", chosen_move.square, chosen_move.value);
        }
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

    /// Return player type description string.
    pub fn type_string(&self) -> String {
        if self.human {
            "Human   ".to_owned()
        } else {
            "Computer".to_owned()
        }
    }
}

impl fmt::Display for Player {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(
            f,
            "{} | {} | Moves: {}",
            self.disk,
            self.type_string(),
            self.rounds_played
        )
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_new_player() {
        let settings = PlayerSettings::default();
        let player = Player::new(Disk::Black, settings);
        assert_eq!(player.disk, Disk::Black);
        assert!(player.human);
        assert!(player.can_play);
        assert_eq!(player.rounds_played, 0);
        assert_eq!(player.settings, settings);
    }

    #[test]
    fn test_reset_player() {
        let mut player = Player::new(Disk::White, PlayerSettings::default());
        player.can_play = false;
        player.rounds_played = 10;
        player.reset();
        assert!(player.can_play);
        assert_eq!(player.rounds_played, 0);
    }

    #[test]
    fn test_set_human() {
        let mut player = Player::new(Disk::Black, PlayerSettings::default());
        player.set_human(false);
        assert!(!player.human);
    }

    #[test]
    fn test_player_type_string() {
        let human_player = Player::new(Disk::Black, PlayerSettings::default());
        assert_eq!(human_player.type_string(), "Human   ");

        let mut computer_player = human_player;
        computer_player.set_human(false);
        assert_eq!(computer_player.type_string(), "Computer");
    }
}
